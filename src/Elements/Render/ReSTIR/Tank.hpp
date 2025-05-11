/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RestTank
*/

#pragma once

#include <random>
#include "../../../Maths/Vector3D.hpp"

namespace raytracer {
typedef struct light_sample {
    math::RGBColor radiance;
    double pdf;
} LightSample;

typedef struct restir_tank {
    LightSample sample;
    double weightSum;
    int count;

    restir_tank();
    void add(const LightSample &candidate, double w, std::mt19937 &gen);
    void merge(const struct restir_tank &other, std::mt19937 &gen);
    math::RGBColor estimate() const;
    void clear();
} Tank;
};
