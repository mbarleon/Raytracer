/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#include "Material.hpp"

raytracer::material::material::material() : bsdf(nullptr)
{
}

raytracer::material::material::material(const std::shared_ptr<BSDF> &bsdf) : bsdf(bsdf)
{
}

raytracer::material::BSDFSample raytracer::material::material::sample(const math::Vector3D &wo,
    const math::Intersect &isect, std::mt19937 &rng) const
{
    return bsdf->sample(wo, isect, rng);
}
