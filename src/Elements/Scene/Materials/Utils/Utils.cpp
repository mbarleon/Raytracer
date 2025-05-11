/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Utils
*/

#include "Utils.hpp"
#include <cmath>
#include <algorithm>

math::Vector3D raytracer::material::reflect(const math::Vector3D &I,
    const math::Vector3D &N)
{
    return I - 2.0f * I.dot(N) * N;
}

math::Vector3D raytracer::material::refract(const math::Vector3D &I,
    const math::Vector3D &N, double eta)
{
    const double cosi = std::clamp((-I).dot(N), -1.0, 1.0);
    const double sint2 = eta * eta * (1.0 - cosi * cosi);
    const double cost = sqrt(std::max(0.0, 1.0 - sint2));

    return eta * I + (eta * cosi - cost) * N;
}

double raytracer::material::reflectance(double cosTheta, double iorI,
    double iorTransmitted)
{
    double r0 = (iorI - iorTransmitted) / (iorI + iorTransmitted);

    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosTheta, 5.0f);
}

math::Vector3D raytracer::material::cosineHemisphere(const math::Vector3D &N)
{
    double r1 = getRandomDouble();
    double r2 = getRandomDouble();

    double phi = 2 * M_PI * r1;
    double x = std::cos(phi) * std::sqrt(r2);
    double y = std::sin(phi) * std::sqrt(r2);
    double z = std::sqrt(1.0 - r2);

    math::Vector3D u = N.orthonormal();
    math::Vector3D v = N.cross(u);

    return (u * x + v * y + N * z).normalize();
}
