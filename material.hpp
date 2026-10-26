#pragma once

class Material{
    private :
        double conductivite_;
        double mass_volumique_;
        double capa_ther_massique_;
        static double check_positivity(double value);

    public :
        Material(double conductivite, double mass_volumique, double capa_ther_massique);
       ~Material() = default;
        double diffusivity() const;
        double capa_ther_massique() const;
        double mass_volumique() const;
        double conductivite() const;

};