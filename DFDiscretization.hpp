#pragma once
#include "SpatialDiscretization.hpp"

class DFDiscretization : public SpatialDiscretization {
private:
    TridiagonalMatrix M_;
    TridiagonalMatrix K_;
    std::vector<double> F_;

    void applyBoundaryConditions(const BoundaryCondition& bc_left,
                                 const BoundaryCondition& bc_right,
                                 double dx);

public:
    DFDiscretization() = default;

    void buildOperators(const Mesh1D& mesh, 
                        const Material& mat,
                        const BoundaryCondition& bc_left,
                        const BoundaryCondition& bc_right) override;

    const TridiagonalMatrix& getStiffnessMatrix() const override { return K_; }
    const TridiagonalMatrix& getMassMatrix() const override { return M_; }
    const std::vector<double>& getRhsVector() const override { return F_; }
};