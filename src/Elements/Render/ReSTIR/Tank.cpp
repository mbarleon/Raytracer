/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RestTank
*/

#include "Tank.hpp"

raytracer::Tank::Tank(): weightSum(0.0), count(0)
{
    sample.pdf = 0.0;
}

void raytracer::Tank::add(const LightSample &candidate, double w, std::mt19937 &gen)
{
    const double wNum = w * w;

    weightSum += wNum;
    ++count;

    if (std::uniform_real_distribution<> dist(0.0, 1.0); dist(gen) < wNum / weightSum) {
        sample = candidate;
    }
}

void raytracer::Tank::merge(const struct Tank &other, std::mt19937 &gen)
{
    if (other.weightSum <= 0.0) {
        return;
    }

    weightSum += other.weightSum;
    count += other.count;

    if (std::uniform_real_distribution<> dist(0.0, 1.0); dist(gen) < other.weightSum / weightSum) {
        sample = other.sample;
    }
}

math::RGBColor raytracer::Tank::estimate() const
{
    return sample.radiance;
}

void raytracer::Tank::clear()
{
    sample.radiance = math::RGBColor(0);
    sample.pdf = 0.0;
    weightSum = 0.0;
    count = 0;
}
