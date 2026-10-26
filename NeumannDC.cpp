#include "NeumannDC.hpp"

NeumannDC::NeumannDC( BoundarySide side,double flux)
    :BoundaryCondition(side), prescribed_flux_(flux){

}

double NeumannDC::computeBoundaryValue(double neighbor_temp,double dx) const {

    if(side_ == BoundarySide::LEFT){

        // neighbor_temp recu vaut u[1]
        // u[0] = u[1] - flux * dx
        return neighbor_temp -  prescribed_flux_*dx ;

    }
    else{

        // neighbor_temp recu vaut u[N - 2]
        // u[N - 1] = u[N - 2] + flux * dx
        return neighbor_temp+ prescribed_flux_ *dx;
    }

}

/* quand on appelera ces methodes ca sera en mode : 
// Agrégation de deux conditions polymorphes :
    // Bord gauche : Dirichlet à 100°C
    std::unique_ptr<BoundaryCondition> bc_left = 
        std::make_unique<DirichletBC>(BoundarySide::LEFT, 100.0);

    // Bord droit : Neumann (isolé / adiabatique, flux = 0)
    std::unique_ptr<BoundaryCondition> bc_right = 
        std::make_unique<NeumannBC>(BoundarySide::RIGHT, 0.0);

    double dx = 0.01;
    double u_1 = 50.0;     // température du voisin intérieur gauche
    double u_N_minus_1 = 30.0; // température du voisin intérieur droit

    // Appel polymorphe : le code appelle la bonne version sans if/else
    double u_0 = bc_left->computeBoundaryValue(u_1, dx);          // Vaut 100.0
    double u_N = bc_right->computeBoundaryValue(u_N_minus_1, dx);  // Vaut 30.0

    std::cout << "Bord gauche (Dirichlet) : " << u_0 << " °C\n";
    std::cout << "Bord droit (Neumann) : " << u_N << " °C\n";
*/