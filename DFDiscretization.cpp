#include "DFDiscretization.hpp"
#include "DirichletDC.hpp"
#include "NeumannDC.hpp"

void DFDiscretization::buildOperators(const Mesh1D& mesh, 
                                      const Material& mat,
                                      const BoundaryCondition& bc_left,
                                      const BoundaryCondition& bc_right) {
    std::size_t n = mesh.node_count();
    double dx = mesh.dx();
    double alpha = mat.diffusivity();
    double inv_dx2 = 1.0 / (dx * dx);

    // 1. En FDM, la dérivée temporelle est locale : M est l'identité
    M_.diag.assign(n, 1.0);
    M_.lower.assign(n - 1, 0.0);
    M_.upper.assign(n - 1, 0.0);

    // 2. Opérateur spatial : -alpha * d²u/dx²
    // (u_i - u_{i-1}) / dx²  et  (u_i - u_{i+1}) / dx²
    K_.diag.assign(n, 2.0 * alpha * inv_dx2);
    K_.lower.assign(n - 1, -alpha * inv_dx2);
    K_.upper.assign(n - 1, -alpha * inv_dx2);

    // 3. Second membre initialement nul
    F_.assign(n, 0.0);

    // 4. Adaptation des bords
    applyBoundaryConditions(bc_left, bc_right, dx);
}

void DFDiscretization::applyBoundaryConditions(const BoundaryCondition& bc_left,
                                               const BoundaryCondition& bc_right,
                                               double dx) {
    std::size_t n = M_.diag.size();

    // --- Bord gauche (i = 0) ---
    if (auto dirichlet = dynamic_cast<const DirichletDC*>(&bc_left)) {
        // Équation forcée : 1 * u_0 = T_fixe (pas de terme temporel)
        M_.diag[0] = 0.0;
        K_.diag[0] = 1.0;
        K_.upper[0] = 0.0;
        F_[0] = dirichlet->computeBoundaryValue(0.0, dx);
    } else if (dynamic_cast<const NeumannDC*>(&bc_left)) {
        // Dérivée nulle ou flux imposé : (u_1 - u_0)/dx = flux
        // -> -1 * u_0 + 1 * u_1 = flux * dx
        M_.diag[0] = 0.0;
        K_.diag[0] = -1.0;
        K_.upper[0] = 1.0;
        F_[0] = bc_left.computeBoundaryValue(0.0, dx); // Renvoie flux * dx
    }

    // --- Bord droit (i = n - 1) ---
    if (auto dirichlet = dynamic_cast<const DirichletDC*>(&bc_right)) {
        // Équation forcée : 1 * u_{n-1} = T_fixe
        M_.diag[n - 1] = 0.0;
        K_.diag[n - 1] = 1.0;
        K_.lower[n - 2] = 0.0;
        F_[n - 1] = dirichlet->computeBoundaryValue(0.0, dx);
    } else if (dynamic_cast<const NeumannDC*>(&bc_right)) {
        // (u_{n-1} - u_{n-2})/dx = flux -> -1 * u_{n-2} + 1 * u_{n-1} = flux * dx
        M_.diag[n - 1] = 0.0;
        K_.lower[n - 2] = -1.0;
        K_.diag[n - 1] = 1.0;
        F_[n - 1] = bc_right.computeBoundaryValue(0.0, dx);
    }
}