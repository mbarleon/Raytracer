/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RestTank
*/

#include <random>
#include "../../Scene/Material/Color.hpp"

namespace raytracer {
typedef struct light_sample {
    RGBColor radiance;
    double pdf;
} LightSample;

typedef struct restir_tank {
    LightSample sample;
    double weightSum;
    int count;

    restir_tank();
    void add(const LightSample &candidate, double w, std::mt19937 &gen);
    void merge(const struct restir_tank &other, std::mt19937 &gen);
    RGBColor estimate() const;
} ReSTIR_Tank;
};
