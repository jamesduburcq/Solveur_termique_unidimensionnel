#include "IE_Solver.hpp"

void IE_Solver::setup(double dt) {
    const auto& M = spatial_.getMassMatrix();
    const auto& K = spatial_.getStiffnessMatrix();

    std::size_t n = M.diag.size();

    // Allocation des 3 vecteurs de la matrice tridiagonale A
    A_.diag.resize(n);
    A_.lower.resize(n - 1);
    A_.upper.resize(n - 1);

    // Diagonale principale (taille N)
    for (std::size_t i = 0; i < n; ++i) {
        A_.diag[i] = M.diag[i] + dt * K.diag[i];
    }

    // Diagonales inférieure et supérieure (taille N - 1)
    for (std::size_t i = 0; i < n - 1; ++i) {
        A_.lower[i] = M.lower[i] + dt * K.lower[i];
        A_.upper[i] = M.upper[i] + dt * K.upper[i];
    }
};

void IE_Solver::step(Mesh1D& mesh,double dt){
    
    const auto& M = spatial_.getMassMatrix();
    const auto& F = spatial_.getRhsVector();
    const auto& u_current = mesh.temperatures();
    std::size_t n = u_current.size();

    // 1. Calcul du second membre : B = M * u^n + dt * F
    std::vector<double> Mu = multiply_matrix_vector(M, u_current);
    std::vector<double> B(n);
    for (std::size_t i = 0; i < n; ++i) {
        B[i] = Mu[i] + dt * F[i];
    }

    // 2. Inversion du système A * u^{n+1} = B
    std::vector<double> u_next = solve_tridiagonal(A_, B);

    // 3. Mise à jour physique
    mesh.set_temperatures(u_next);

};
std::vector<double> IE_Solver::multiply_matrix_vector(const TridiagonalMatrix& mat, 
                                                              const std::vector<double>& vec) const {
    const std::size_t n = vec.size();
    std::vector<double> res(n, 0.0);

    if (n == 0) return res;
    if (n == 1) {
        res[0] = mat.diag[0] * vec[0];
        return res;
    }

    // Premier nœud (bord gauche)
    res[0] = mat.diag[0] * vec[0] + mat.upper[0] * vec[1];

    // Noeuds intérieurs
    for (std::size_t i = 1; i < n - 1; ++i) {
        res[i] = mat.lower[i - 1] * vec[i - 1] 
               + mat.diag[i] * vec[i] 
               + mat.upper[i] * vec[i + 1];
    }

    // Dernier nœud (bord droit)
    res[n - 1] = mat.lower[n - 2] * vec[n - 2] + mat.diag[n - 1] * vec[n - 1];

    return res;
}

std::vector<double> IE_Solver::solve_tridiagonal(const TridiagonalMatrix& A, 
                                                          const std::vector<double>& rhs) const {
    const std::size_t n = A.diag.size();

    if (n == 0) return {};
    if (n == 1) {
        if (A.diag[0] == 0.0) throw std::runtime_error{"Pivot nul à l'indice 0."};
        return {rhs[0] / A.diag[0]};
    }

    std::vector<double> c_prime(n - 1, 0.0);
    std::vector<double> d_prime(n, 0.0);
    std::vector<double> x(n, 0.0);

    // --- Élimination avant ---
    if (A.diag[0] == 0.0) throw std::runtime_error{"Pivot nul à l'indice 0."};
    c_prime[0] = A.upper[0] / A.diag[0];
    d_prime[0] = rhs[0] / A.diag[0];

    for (std::size_t i = 1; i < n; ++i) {
        double denom = A.diag[i] - A.lower[i - 1] * c_prime[i - 1];
        if (denom == 0.0) {
            throw std::runtime_error{"Pivot nul pendant l'élimination de Thomas."};
        }

        if (i < n - 1) {
            c_prime[i] = A.upper[i] / denom;
        }
        d_prime[i] = (rhs[i] - A.lower[i - 1] * d_prime[i - 1]) / denom;
    }

    // --- Substitution arrière ---
    x[n - 1] = d_prime[n - 1];
    for (std::size_t i = n - 1; i > 0; --i) {
        x[i - 1] = d_prime[i - 1] - c_prime[i - 1] * x[i];
    }

    return x;
}