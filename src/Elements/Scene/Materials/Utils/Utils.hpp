/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Utils
*/

#pragma once

#include "../../../../Maths/Vector3D.hpp"

namespace raytracer::material {

// Reflect
math::Vector3D reflect(const math::Vector3D &incident, const math::Vector3D &normal);
bool refract(const math::Vector3D &incident, const math::Vector3D &normal,
    double iorIncident, double iorTransmitted, math::Vector3D &refracted);
double reflectance(double cosTheta, double iorIncident, double iorTransmitted);

void buildOrthonormalBasis(const math::Vector3D &normal, math::Vector3D &tangent,
    math::Vector3D &bitangent);

// Random
double getRandomDouble();
double getRandomDouble(double min, double max);
math::Vector3D getRandomVector();
math::Vector3D getRandomVector(double min, double max);

};
