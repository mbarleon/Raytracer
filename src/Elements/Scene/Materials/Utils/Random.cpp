/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Random
*/

#include "Utils.hpp"

double raytracer::material::getRandomDouble()
{
    return rand() / (RAND_MAX + 1.0);
}

double raytracer::material::getRandomDouble(double min, double max)
{
    return min + (max - min) * getRandomDouble();
}

math::Vector3D raytracer::material::getRandomVector()
{
    return math::Vector3D(getRandomDouble(), getRandomDouble(), getRandomDouble());
}

math::Vector3D raytracer::material::getRandomVector(double min, double max)
{
    return math::Vector3D(getRandomDouble(min, max), getRandomDouble(min, max),
        getRandomDouble(min, max));
}
