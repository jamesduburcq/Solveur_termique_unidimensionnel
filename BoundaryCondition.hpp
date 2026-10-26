#pragma once
enum class BoundarySide{
    LEFT,
    RIGHT
};

class BoundaryCondition {
    protected :
        BoundarySide side_;

    public : 
            explicit BoundaryCondition(BoundarySide side) 
                : side_{side} {}
            virtual ~BoundaryCondition()=default;

            BoundarySide getSide() const {
                return side_;            }

            virtual double computeBoundaryValue( double neighbor_temp, double dx) const = 0;

};