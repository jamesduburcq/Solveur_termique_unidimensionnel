#pragma once
#include <vector>
#include <memory>
#include "BoundaryCondition.hpp"
#include "material.hpp"
#include "mesh_1D.hpp"


struct TridiagonalMatrix {
    std::vector<double> lower; // taille N-1
    std::vector<double> diag;  // taille N
    std::vector<double> upper; // taille N-1
};

class SpatialDiscretization {
public:
    virtual ~SpatialDiscretization() = default;

    // Construit les opérateurs selon la méthode
    virtual void buildOperators(const Mesh1D& mesh, 
                                const Material& mat,
                                const BoundaryCondition& bc_left,
                                const BoundaryCondition& bc_right) = 0;

    // Accès aux opérateurs
    virtual const TridiagonalMatrix& getStiffnessMatrix() const = 0;
    virtual const TridiagonalMatrix& getMassMatrix() const = 0;
    virtual const std::vector<double>& getRhsVector() const = 0;
};