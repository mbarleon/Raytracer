/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RestTank
*/

#include "ReSTIR_Tank.hpp"

raytracer::restir_tank::restir_tank(): weightSum(0.0), count(0)
{
}

void raytracer::restir_tank::add(const LightSample &candidate, double w, std::mt19937 &gen)
{
    ++count;
    weightSum += w;
    std::uniform_real_distribution<> dist(0.0, 1.0);
    if (dist(gen) * weightSum < w) {
        sample = candidate;
    }
}

void raytracer::restir_tank::merge(const struct restir_tank &other, std::mt19937 &gen)
{
    if (other.count == 0) {
        return;
    }
    const double w = other.weightSum;
    add(other.sample, w, gen);
}

raytracer::RGBColor raytracer::restir_tank::estimate() const
{
    return sample.radiance;
}
