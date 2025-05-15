/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Utils
*/

#pragma once

#include "../../../../Maths/Vector3D.hpp"
#include <random>

namespace raytracer::material {

// Reflect
math::Vector3D reflect(const math::Vector3D &I, const math::Vector3D &N);
math::Vector3D refract(const math::Vector3D &I, const math::Vector3D &N, const double eta);
double reflectance(const double cosTheta, const double iorI, const double iorTransmitted);

math::Vector3D cosineHemisphere(const math::Vector3D &N, std::mt19937 &rng);

// Random
double getRandomDouble(std::mt19937 &rng);
double getRandomDouble(std::mt19937 &rng, const double min, const double max);
math::Vector3D getRandomVector(std::mt19937 &rng);
math::Vector3D getRandomVector(std::mt19937 &rng, const double min, const double max);
std::mt19937 getRng(const unsigned threadId, const unsigned res_x, const unsigned res_y);

}
