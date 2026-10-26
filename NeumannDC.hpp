#include "BoundaryCondition.hpp"

class NeumannDC : public BoundaryCondition{
    private:
        double prescribed_flux_;
    public :
    explicit NeumannDC( BoundarySide side, double flux = 0.0);
                
    double computeBoundaryValue(double neighbor_temp, double dx) const override;
    
    
};