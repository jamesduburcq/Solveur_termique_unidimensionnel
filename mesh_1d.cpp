#include "mesh_1d.hpp"


double Mesh1D::check_length(double length){
    if (length <= 0){
        throw std::invalid_argument{"La longueur ne peut pas être nulle ou négative"};
    }
    return length;
}

std::size_t Mesh1D::check_nodes(std::size_t nodes){
    if (nodes < 2){
        throw std::invalid_argument{"nodes_ doit être supérieur ou égal à 2"};
    }
    return nodes;
}




Mesh1D::Mesh1D(double length, std::size_t node_count, double initial_temp)
:length_{check_length(length)},node_count_{check_nodes(node_count)}, dx_{length_ /(node_count_ - 1)} , temperatures_(node_count_, initial_temp)
{}



double Mesh1D::coordinate_at(std::size_t index) const {
    if (index >= node_count_) {
        throw std::out_of_range{"Indice de noeud hors limites."};
    }
    return index * dx_;
}

double Mesh1D::operator[](std::size_t index) const{
    return temperatures_.at(index);
}



double& Mesh1D::operator[](std::size_t index){
    return temperatures_.at(index);
}

void Mesh1D::initialize(const std::function<double(double)>& profile) {
    for (std::size_t i = 0; i < node_count_; ++i) {
        temperatures_[i] = profile(coordinate_at(i));
    }
}

void Mesh1D::set_temperatures(const std::vector<double>& new_temps) {
    if (new_temps.size() != node_count_) {
        throw std::invalid_argument{"La taille du vecteur ne correspond pas au maillage."};
    }
    temperatures_ = new_temps;
}

