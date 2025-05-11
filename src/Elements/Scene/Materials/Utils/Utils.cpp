/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Utils
*/

#include "Utils.hpp"
#include <cmath>

math::Vector3D raytracer::material::reflect(const math::Vector3D &incident,
    const math::Vector3D &normal)
{
    return incident - 2.0f * incident.dot(normal) * normal;
}

bool raytracer::material::refract(const math::Vector3D &incident,
    const math::Vector3D &normal, double iorIncident, double iorTransmitted,
    math::Vector3D &refracted)
{
    const double eta = iorIncident / iorTransmitted;
    const double cosThetaI = -normal.dot(incident);
    const double sin2ThetaT = eta * eta * (1.0f - cosThetaI * cosThetaI);

    if (sin2ThetaT > 1.0f) {
        return false;
    }
    const double cosThetaT = std::sqrt(1.0f - sin2ThetaT);
    refracted = eta * incident + (eta * cosThetaI - cosThetaT) * normal;
    return true;
}

double raytracer::material::reflectance(double cosTheta, double iorIncident,
    double iorTransmitted)
{
    double r0 = (iorIncident - iorTransmitted) / (iorIncident + iorTransmitted);

    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosTheta, 5.0f);
}

void raytracer::material::buildOrthonormalBasis(const math::Vector3D &normal,
    math::Vector3D &tangent, math::Vector3D &bitangent)
{
    if (std::fabs(normal._x) > std::fabs(normal._z)) {
        tangent = math::Vector3D(-normal._y, normal._x, 0.0).normalize();
    } else {
        tangent = math::Vector3D(0.0, -normal._z, normal._y).normalize();
    }
    bitangent = normal.cross(tangent);
}
