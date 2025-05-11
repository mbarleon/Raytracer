/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Dielectric
*/

#include "Dielectric.hpp"
#include <random>

raytracer::material::DielectricBSDF::DielectricBSDF(double etaExt, double etaInt) :
    etaExt(etaExt), etaInt(etaInt)
{
}

raytracer::material::BSDFSample raytracer::material::DielectricBSDF::sample(const math::Vector3D &wo,
    const math::Intersect &isect) const
{
    math::Vector3D normal = isect.normal;
    double cosTheta = (-wo).dot(normal);
    double etaI = etaExt, etaT = etaInt;

    if (cosTheta < 0.0) {
        normal = -normal;
        std::swap(etaI, etaT);
        cosTheta = (-wo).dot(normal);
    }

    double reflectProb;
    math::Vector3D wi;
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    bool cannotRefract = etaI / etaT * sinTheta > 1.0;

    if (cannotRefract || getRandomDouble() < reflectProb) {
        wi = reflect(-wo, normal);
        reflectProb = 1.0;
    } else {
        wi = refract(-wo, normal, etaI / etaT);
        reflectProb = 0.0;
    }
    return {wi, 1.0, isect.object->getColor()};
}
