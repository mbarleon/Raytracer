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
struct LightSample {
    math::RGBColor radiance;
    double pdf;
    bool isDelta = false;
};

struct Tank {
    LightSample sample;
    double weightSum;
    int count;

    Tank();
    void add(const LightSample &candidate, double w, std::mt19937 &gen);
    void merge(const Tank &other, std::mt19937 &gen);
    math::RGBColor estimate() const;
    void clear();
};
}
