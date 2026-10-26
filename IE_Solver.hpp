#pragma once
#include "TimeSolver.hpp"
#include <vector>
class IE_Solver : public TimeSolver{

    private : 
    TridiagonalMatrix A_;
    std::vector<double> multiply_matrix_vector(const TridiagonalMatrix& mat, 
                                                              const std::vector<double>& vec) const;
    std::vector<double> solve_tridiagonal(const TridiagonalMatrix& A, 
                                                          const std::vector<double>& rhs) const ;                                                          
    public : 
    explicit IE_Solver(const SpatialDiscretization& spatial)
        : TimeSolver(spatial) {}
       void setup(double dt) override;
        void step(Mesh1D& mesh, double dt) override;
};  