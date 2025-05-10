/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Random
*/

#include "../Pathtracer.hpp"

double raytracer::getRandomDouble()
{
    return rand() / (RAND_MAX + 1.0);
}

double raytracer::getRandomDouble(double min, double max)
{
    return min + (max - min) * getRandomDouble();
}

math::Vector3D raytracer::getRandomVector()
{
    return math::Vector3D(getRandomDouble(), getRandomDouble(), getRandomDouble());
}

math::Vector3D raytracer::getRandomVector(double min, double max)
{
    return math::Vector3D(getRandomDouble(min, max), getRandomDouble(min, max),
        getRandomDouble(min, max));
}
