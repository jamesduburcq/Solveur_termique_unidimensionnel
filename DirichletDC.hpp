#include "BoundaryCondition.hpp"

class DirichletDC : public BoundaryCondition { 

    private : 
        double fixed_temperature_;
    
    public :
    DirichletDC(BoundarySide side, double temperature)
            :BoundaryCondition(side), fixed_temperature_(temperature){}

    double computeBoundaryValue( double neighbor_temp, double dx) const override{
        return fixed_temperature_;
    }
};