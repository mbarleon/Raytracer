/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Random
*/

#include "Utils.hpp"

double raytracer::material::getRandomDouble(std::mt19937 &rng)
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

double raytracer::material::getRandomDouble(std::mt19937 &rng, double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

math::Vector3D raytracer::material::getRandomVector(std::mt19937 &rng)
{
    return math::Vector3D(getRandomDouble(rng), getRandomDouble(rng), getRandomDouble(rng));
}

math::Vector3D raytracer::material::getRandomVector(std::mt19937 &rng, double min, double max)
{
    return math::Vector3D(getRandomDouble(rng, min, max), getRandomDouble(rng, min, max),
        getRandomDouble(rng, min, max));
}
