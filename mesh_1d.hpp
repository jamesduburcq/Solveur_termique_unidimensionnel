#pragma once

#include <vector>
#include <cstddef>
#include <functional>
#include <stdexcept>

class Mesh1D {
private:
    double length_;
    std::size_t node_count_;
    double dx_;
    std::vector<double> temperatures_;

    static double check_length(double length);
    static std::size_t check_nodes(std::size_t nodes);

public:
    Mesh1D(double length, std::size_t node_count, double initial_temp = 0.0);

    // Géométrie
    double length() const { return length_; }
    std::size_t node_count() const { return node_count_; }
    double dx() const { return dx_; }
    double coordinate_at(std::size_t index) const;

    // Accès aux valeurs
    double operator[](std::size_t index) const;
    double& operator[](std::size_t index);
    const std::vector<double>& temperatures() const { return temperatures_; }

    // Initialisation
    void initialize(const std::function<double(double)>& profile);
    void set_temperatures(const std::vector<double>& new_temps);
};