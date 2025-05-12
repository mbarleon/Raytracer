/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RestTank
*/

#include "Tank.hpp"

raytracer::restir_tank::restir_tank(): weightSum(0.0), count(0)
{
}

void raytracer::restir_tank::add(const LightSample &candidate, double w, std::mt19937 &gen)
{
    weightSum += w;
    ++count;

    std::uniform_real_distribution<> dist(0.0, 1.0);
    if (dist(gen) < w / weightSum) {
        sample = candidate;
    }
}

void raytracer::restir_tank::merge(const struct restir_tank &other, std::mt19937 &gen)
{
    if (other.weightSum <= 0.0) {
        return;
    }

    weightSum += other.weightSum;
    count += other.count;

    std::uniform_real_distribution<> dist(0.0, 1.0);
    if (dist(gen) < other.weightSum / weightSum) {
        sample = other.sample;
    }
}

math::RGBColor raytracer::restir_tank::estimate() const
{
    return sample.radiance;
}

void raytracer::restir_tank::clear()
{
    sample.radiance = math::RGBColor(0);
    sample.pdf = 0.0;
    weightSum = 0.0;
    count = 0;
}
