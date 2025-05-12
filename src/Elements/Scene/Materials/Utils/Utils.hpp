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
math::Vector3D refract(const math::Vector3D &I, const math::Vector3D &N, double eta);
double reflectance(double cosTheta, double iorI, double iorTransmitted);

math::Vector3D cosineHemisphere(const math::Vector3D &N, std::mt19937 &rng);

// Random
double getRandomDouble(std::mt19937 &rng);
double getRandomDouble(std::mt19937 &rng, double min, double max);
math::Vector3D getRandomVector(std::mt19937 &rng);
math::Vector3D getRandomVector(std::mt19937 &rng, double min, double max);
std::mt19937 getRng(unsigned threadId, unsigned res_x, unsigned res_y);

};
