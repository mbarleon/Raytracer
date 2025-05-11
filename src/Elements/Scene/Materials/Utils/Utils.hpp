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
math::Vector3D reflect(const math::Vector3D &I, const math::Vector3D &N);
math::Vector3D refract(const math::Vector3D &I, const math::Vector3D &N, double eta);
double reflectance(double cosTheta, double iorI, double iorTransmitted);

math::Vector3D cosineHemisphere(const math::Vector3D &N);

// Random
double getRandomDouble();
double getRandomDouble(double min, double max);
math::Vector3D getRandomVector();
math::Vector3D getRandomVector(double min, double max);

};
