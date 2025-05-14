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
#include "Macro.hpp"

raytracer::material::DielectricBSDF::DielectricBSDF(double etaExt, double etaInt) :
    _etaExt(etaExt), _etaInt(etaInt)
{
}

raytracer::material::BSDFSample raytracer::material::DielectricBSDF::sample(const math::Vector3D &wo,
    const math::Intersect &isect, std::mt19937 &rng) const
{
    const bool entering = (wo.dot(isect.normal) < 0);
    const math::Vector3D N = entering ? isect.normal : -isect.normal;
    const double eta_i = entering ? _etaInt : _etaExt;
    const double eta_t = entering ? _etaExt : _etaInt;
    const double eta   = eta_i / eta_t;

    const math::Vector3D V = wo.normalize();
    const double cosi = std::clamp((-V).dot(N), 0.0, 1.0);
    const double k = 1.0 - eta*eta * (1.0 - cosi*cosi);

    math::Vector3D refracted;
    bool hasRefract = false;
    if (k >= 0.0) {
        refracted = (eta * (V + cosi * N) - std::sqrt(k) * N).normalize();
        hasRefract = true;
    }

    // schlick
    const double R0 = ((eta_t - eta_i)/(eta_t + eta_i));
    const double reflect_prob = R0*R0 + (1.0 - R0*R0) * std::pow(1.0 - cosi, 5.0);

    // pick random choice reflect / refract
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    const bool doReflect = (!hasRefract) || (dist(rng) < reflect_prob);

    const math::Vector3D wi = doReflect ? reflect(V, N).normalize() : refracted;
    const math::RGBColor beta = math::RGBColor(1.0);

    return { wi, 1.0, beta, true };
}

math::RGBColor raytracer::material::DielectricBSDF::evaluate(const math::Vector3D __attribute__((unused)) &wo,
    const math::Vector3D __attribute__((unused)) &wi, const math::Intersect __attribute__((unused)) &isect,
    std::mt19937 __attribute__((unused)) &rng) const
{
    const bool entering = (wo.dot(isect.normal) < 0.0);
    const math::Vector3D N = entering ?  isect.normal : -isect.normal;
    const double eta_i = entering ? _etaInt : _etaExt;
    const double eta_t = entering ? _etaExt : _etaInt;
    const double eta = eta_i / eta_t;

    const math::Vector3D V = wo.normalize();
    const double cosi = std::clamp((-V).dot(N), 0.0, 1.0);
    const double k = 1.0 - eta*eta * (1.0 - cosi*cosi);

    // schlick
    const double R0 = (eta_t - eta_i) / (eta_t + eta_i);
    const double F  = R0*R0 + (1.0 - R0*R0) * std::pow(1.0 - cosi, 5.0);

    const math::Vector3D R = reflect(V, N).normalize();
    const math::Vector3D T = (eta * (V + cosi * N) - std::sqrt(k) * N).normalize();

    if (k < 0.0) {
        if ((wi - R).length() < EPSILON) {
            const double invCos = 1.0 / std::max(cosi, EPSILON);
            return math::RGBColor(invCos);
        }
        return math::RGBColor(0.0);
    }

    if ((wi - R).length() < EPSILON) {
        // f = F / |cosθᵢ| · δ(ωᵢ - R)
        const double invCos = 1.0 / std::max(cosi, EPSILON);
        return math::RGBColor(F * invCos);
    }
    if ((wi - T).length() < EPSILON) {
        // f = (1-F)·η² / |cosθᵢ| · δ(ωᵢ - T)
        const double invCos = 1.0 / std::max(cosi, EPSILON);
        return math::RGBColor((1.0 - F) * (eta*eta) * invCos);
    }
    return math::RGBColor(0.0);
}
