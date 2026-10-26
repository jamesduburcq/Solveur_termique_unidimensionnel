#pragma once

#include <vector>
#include "mesh_1D.hpp"
#include "SpatialDIscretization.hpp"

class TimeSolver {
protected:
    // Agrégation : référence vers la discrétisation spatiale qui fournit M, K et F
    const SpatialDiscretization& spatial_;

public:
    explicit TimeSolver(const SpatialDiscretization& spatial)
        : spatial_{spatial} {}

    virtual ~TimeSolver() = default;

    // Prépare d'éventuels opérateurs constants liés au pas de temps dt (ex: factorisation de matrice)
    virtual void setup(double dt) = 0;

    // Effectue l'avancement d'un pas de temps dt et met à jour le maillage
    virtual void step(Mesh1D& mesh, double dt) = 0;

    // Accesseur vers la discrétisation spatiale associée
    const SpatialDiscretization& spatial_discretization() const {
        return spatial_;
    }
};