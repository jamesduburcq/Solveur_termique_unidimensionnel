#include <iostream>
#include <fstream>
#include "material.hpp"
#include "mesh_1d.hpp"
#include "DirichletDC.hpp"
#include "NeumannDC.hpp"
#include "DFDiscretization.hpp"
#include "IE_Solver.hpp"

// Macro pour exécuter une simulation et la sauvegarder sans répéter 6 fois le même code
#define RUN_SIMULATION(NOM_FICHIER, MESH_OBJ, MAT_OBJ, BC_LEFT, BC_RIGHT, DT, T_MAX, SAVE_INTERVAL) \
{ \
    DFDiscretization spatial; \
    spatial.buildOperators(MESH_OBJ, MAT_OBJ, BC_LEFT, BC_RIGHT); \
    IE_Solver solver(spatial); \
    solver.setup(DT); \
    std::ofstream out(NOM_FICHIER); \
    out << "Temps"; \
    for(std::size_t i = 0; i < MESH_OBJ.node_count(); ++i) out << "," << MESH_OBJ.coordinate_at(i); \
    out << "\n"; \
    int num_steps = static_cast<int>(T_MAX / DT); \
    for (int n = 1; n <= num_steps; ++n) { \
        solver.step(MESH_OBJ, DT); \
        if (n % SAVE_INTERVAL == 0) { \
            out << n * DT; \
            for(std::size_t i = 0; i < MESH_OBJ.node_count(); ++i) out << "," << MESH_OBJ[i]; \
            out << "\n"; \
        } \
    } \
    out.close(); \
    std::cout << " -> " << NOM_FICHIER << " termine." << std::endl; \
}

int main() {
    std::cout << "Lancement de la campagne de tests..." << std::endl;

    // Définition de 3 matériaux très différents
    Material cuivre(400.0, 8960.0, 385.0);       // Très bon conducteur
    Material inox(15.0, 8000.0, 500.0);          // Conducteur moyen
    Material beton(1.0, 2300.0, 880.0);          // Isolant (mauvais conducteur)

    // =========================================================================
    // TEST A : Influence du matériau (Vitesse de propagation)
    // 100°C à gauche, 20°C à droite. On regarde à t = 5000 secondes.
    // =========================================================================
    Mesh1D meshA1(1.0, 50, 20.0); Mesh1D meshA2(1.0, 50, 20.0); Mesh1D meshA3(1.0, 50, 20.0);
    DirichletDC dir_100_L(BoundarySide::LEFT, 100.0);
    DirichletDC dir_20_R(BoundarySide::RIGHT, 20.0);
    
    RUN_SIMULATION("testA_cuivre.csv", meshA1, cuivre, dir_100_L, dir_20_R, 50.0, 5000.0, 10);
    RUN_SIMULATION("testA_inox.csv",   meshA2, inox,   dir_100_L, dir_20_R, 50.0, 5000.0, 10);
    RUN_SIMULATION("testA_beton.csv",  meshA3, beton,  dir_100_L, dir_20_R, 50.0, 5000.0, 10);

    // =========================================================================
    // TEST B : Accumulation (Dirichlet vs Neumann nul) avec du Cuivre
    // 100°C à gauche, Isolé à droite. Simulation longue (20 000 s).
    // =========================================================================
    Mesh1D meshB(1.0, 50, 20.0);
    NeumannDC neu_0_R(BoundarySide::RIGHT, 0.0); // Bord isolé
    RUN_SIMULATION("testB_accu.csv", meshB, cuivre, dir_100_L, neu_0_R, 100.0, 20000.0, 20);

    // =========================================================================
    // TEST C : Refroidissement asymétrique (Cuivre)
    // Barre initialement à 100°C. 20°C à gauche, Isolé à droite.
    // =========================================================================
    Mesh1D meshC(1.0, 50, 100.0); // Initiale à 100°C
    DirichletDC dir_20_L(BoundarySide::LEFT, 20.0);
    RUN_SIMULATION("testC_refroid.csv", meshC, cuivre, dir_20_L, neu_0_R, 100.0, 15000.0, 15);

    // =========================================================================
    // TEST D : Injection d'un flux de chaleur constant (Inox)
    // Flux entrant à gauche (ex: un laser), 20°C maintenu à droite.
    // =========================================================================
    Mesh1D meshD(1.0, 50, 20.0);
    // Note : Selon la convention de votre code, un flux entrant à gauche peut être 
    // négatif ou positif. S'il refroidit au lieu de chauffer, enlevez le "-".
    NeumannDC neu_flux_L(BoundarySide::LEFT, 5000.0); 
    RUN_SIMULATION("testD_flux.csv", meshD, inox, neu_flux_L, dir_20_R, 200.0, 50000.0, 25);

    std::cout << "Tous les tests sont termines !" << std::endl;
    return 0;
}