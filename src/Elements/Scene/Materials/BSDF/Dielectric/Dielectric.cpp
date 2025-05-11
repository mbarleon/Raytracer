/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Dielectric
*/

#include "Dielectric.hpp"
#include "../../../../../Maths/Intersect.hpp"
#include <random>
#include <algorithm>

raytracer::material::DielectricBSDF::DielectricBSDF(double etaExt, double etaInt) :
    etaExt(etaExt), etaInt(etaInt)
{
}

raytracer::material::BSDFSample raytracer::material::DielectricBSDF::sample(const math::Vector3D &wo,
    const math::Intersect &isect) const
{
    const math::Vector3D N = isect.normal;
    const bool entering = wo.dot(N) > 0.0;
    const math::Vector3D normal = entering ? N : -N;

    const double eta = entering ? etaExt / etaInt : etaInt / etaExt;

    const double cosThetaI = std::clamp(wo.normalize().dot(normal), -1.0, 1.0);
    const double fresnel = reflectance(std::abs(cosThetaI), etaExt, etaInt);

    if (getRandomDouble(0.0, 1.0) < fresnel) {
        // réflexion spéculaire
        const math::Vector3D reflected = reflect(-wo, normal).normalize();
        return {reflected, fresnel, math::RGBColor(1.0)};
    }

    // réfraction
    const math::Vector3D refracted = refract(-wo, normal, eta).normalize();

    // absorption via Beer-Lambert
    double distance = 1.0; // ou un chemin moyen estimé
    const math::RGBColor mat = isect.object->getColor();
    math::RGBColor absorption = math::RGBColor(
        std::exp(-mat._x * distance),
        std::exp(-mat._y * distance),
        std::exp(-mat._z * distance)
    );
    return {refracted, 1.0 - fresnel, absorption};
}

math::RGBColor raytracer::material::DielectricBSDF::evaluate(const math::Vector3D __attribute__((unused)) &wo,
    const math::Vector3D __attribute__((unused)) &wi, const math::Intersect __attribute__((unused)) &isect) const
{
    return math::RGBColor(0.0);
}
