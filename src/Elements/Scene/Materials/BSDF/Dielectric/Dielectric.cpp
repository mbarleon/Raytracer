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
    const math::Intersect &isect, std::mt19937 &rng) const
{
    const math::Vector3D N = isect.normal;
    const bool entering = (wo.dot(N) > 0);
    const math::Vector3D normal = entering ? N : -N;
    const double eta = entering ? etaExt / etaInt : etaInt / etaExt;

    const double cosi = std::clamp(wo.normalize().dot(normal), -1.0, 1.0);
    const double F = reflectance(fabs(cosi), etaExt, etaInt);

    math::Vector3D wi;
    double pdf;
    math::RGBColor beta;

    // choice reflrect or refract
    if (getRandomDouble(rng) < F) {
        wi = reflect(-wo, normal).normalize();
        pdf = F;
        beta = math::RGBColor(1.0);
    } else {
        wi = refract(-wo, normal, eta).normalize();
        const double cosThetaI = fabs(wo.dot(normal));
        const double cosThetaT = fabs(wi.dot(normal));
        const double jac = (eta*eta * cosThetaT) / cosThetaI;
        pdf = (1 - F) * jac;

        // Bee­r–Lambert
        const double distance = 1.0;
        const math::RGBColor mat = isect.object->getColor();
        beta = math::RGBColor(
            exp(-mat._x*distance),
            exp(-mat._y*distance),
            exp(-mat._z*distance)
        );
    }
    return { wi, pdf, beta };
}

math::RGBColor raytracer::material::DielectricBSDF::evaluate(const math::Vector3D __attribute__((unused)) &wo,
    const math::Vector3D __attribute__((unused)) &wi, const math::Intersect __attribute__((unused)) &isect,
    std::mt19937 __attribute__((unused)) &rng) const
{
    return math::RGBColor(0.0);
}
