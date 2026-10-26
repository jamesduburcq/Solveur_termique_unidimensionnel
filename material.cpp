
#include "material.hpp"


#include <stdexcept>

double Material::check_positivity(double value){
    if (value <= 0) {
        throw std::invalid_argument{"value must be positive"};
    }
    return value;
}

Material::Material(double conductivite, double mass_volumique, double capa_ther_massique)
:conductivite_{check_positivity(conductivite)},mass_volumique_{check_positivity(mass_volumique)}, capa_ther_massique_{check_positivity(capa_ther_massique)}
{}



double Material::diffusivity() const{
    return conductivite_ / (capa_ther_massique_ * mass_volumique_);
}

double Material::capa_ther_massique() const{
    return capa_ther_massique_;
}
double Material::conductivite() const{
    return conductivite_;
}
double Material::mass_volumique() const{
    return mass_volumique_;
}



