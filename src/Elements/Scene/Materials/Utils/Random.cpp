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

double raytracer::material::getRandomDouble(std::mt19937 &rng, const double min, const double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

math::Vector3D raytracer::material::getRandomVector(std::mt19937 &rng)
{
    return math::Vector3D(getRandomDouble(rng), getRandomDouble(rng), getRandomDouble(rng));
}

math::Vector3D raytracer::material::getRandomVector(std::mt19937 &rng, const double min, const double max)
{
    return math::Vector3D(getRandomDouble(rng, min, max), getRandomDouble(rng, min, max),
        getRandomDouble(rng, min, max));
}

std::mt19937 raytracer::material::getRng(const unsigned threadId, const unsigned res_x, const unsigned res_y)
{
    std::seed_seq seq { std::random_device{}(), threadId, res_x, res_y };

    return std::mt19937(seq);
};
