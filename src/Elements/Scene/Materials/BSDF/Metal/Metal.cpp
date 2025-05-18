/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Metal
*/

#include "Metal.hpp"
#include "Macro.hpp"
#include <cmath>

raytracer::material::MetalBSDF::MetalBSDF(const double roughness) : _roughness(roughness)
{
}

static double distributionGGX(double NdotH, double alpha) noexcept
{
    const double a2 = alpha * alpha;
    const double denom = NdotH * NdotH * (a2 - 1.0) + 1.0;

    return a2 / (M_PI * denom * denom);
}

static double geometrySchlickGGX(double NdotV, double alpha) noexcept
{
    double k = (alpha + 1.0);

    k = (k * k) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

static double geometrySmith(double NdotV, double NdotL, double alpha) noexcept
{
    const double ggx1 = geometrySchlickGGX(NdotV, alpha);
    const double ggx2 = geometrySchlickGGX(NdotL, alpha);

    return ggx1 * ggx2;
}

static math::RGBColor fresnelSchlick(double cosTheta, const math::RGBColor &F0) noexcept
{
    // f = F0 + (1−F0)*(1−cosθ)^5
    const double factor = std::pow(1.0 - cosTheta, 5.0);

    return F0 + (math::RGBColor(1.0) - F0) * factor;
}

raytracer::material::BSDFSample raytracer::material::MetalBSDF::sample(const math::Vector3D &wo, const math::Intersect &isect,
    std::mt19937 &rng) const
{
    const math::Vector3D N = isect.normal.normalize();
    const math::Vector3D V = wo.normalize();

    std::uniform_real_distribution<double> u01(0.0, 1.0);
    const double u1 = u01(rng);
    const double u2 = u01(rng);

    const double phi = 2.0 * M_PI * u1;
    const double cosTheta = std::sqrt((1.0 - u2) / (1.0 + (_roughness * _roughness - 1.0) * u2));
    const double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    const math::Vector3D T = N.orthonormal().cross(N).normalize();
    const math::Vector3D B = N.cross(T);
    const math::Vector3D H = (T * (std::cos(phi) * sinTheta) + B * (std::sin(phi) * sinTheta) + N * cosTheta).normalize();

    // perfect reflect
    const math::Vector3D wi = (V - H * 2.0 * V.dot(H)).normalize();

    // pdf = D(H)·(N·H) / (4·(V·H))
    const double NdotH = std::max(N.dot(H), 0.0);
    const double VdotH = std::max(V.dot(H), 0.0);
    const double D = distributionGGX(NdotH, _roughness);
    const double pdf = (D * NdotH) / (4.0 * VdotH + EPSILON);

    const math::RGBColor F = fresnelSchlick(VdotH, isect.object->getColorAt(isect.point));
    const double G = geometrySmith(std::max(N.dot(V), 0.0), std::max(N.dot(wi), 0.0), _roughness);

    // sample radiance = F * D * G / (4 N·V)
    const double NdotL = std::max(N.dot(wi), 0.0);
    const math::RGBColor spec = F * (D * G / (4.0 * std::max(N.dot(V), 0.0) + EPSILON)) * NdotL;
    return {wi, pdf, spec, true};
}

math::RGBColor raytracer::material::MetalBSDF::evaluate(const math::Vector3D &wo, const math::Vector3D &wi,
    const math::Intersect &isect, std::mt19937 __attribute__((unused)) & rng) const
{
    const math::Vector3D N = isect.normal.normalize();
    const math::Vector3D V = wo.normalize();
    const math::Vector3D L = wi.normalize();

    const double NdotV = std::max(N.dot(V), 0.0);
    const double NdotL = std::max(N.dot(L), 0.0);

    if (NdotV <= 0.0 || NdotL <= 0.0)
        return math::RGBColor(0.0);

    const math::Vector3D H = (V + L).normalize();
    const double NdotH = std::max(N.dot(H), 0.0);
    const double VdotH = std::max(V.dot(H), 0.0);

    const double D = distributionGGX(NdotH, _roughness);
    const double G = geometrySmith(NdotV, NdotL, _roughness);
    const math::RGBColor F = fresnelSchlick(VdotH, isect.object->getColorAt(isect.point));

    // brdf = F·D·G / (4·N·V·N·L)
    return F * (D * G / (4.0 * NdotV * NdotL + EPSILON));
}
