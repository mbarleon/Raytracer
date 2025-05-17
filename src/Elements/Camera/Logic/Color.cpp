/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Background
*/

#include "../../Scene/Materials/Utils/Utils.hpp"
#include "Pathtracer.hpp"
#include <algorithm>

math::RGBColor raytracer::getBackgroundColor(const math::Vector3D &v, const math::RGBColor &background)
{
    const math::Vector3D dir = v.normalize();
    const auto t = 0.5 * (dir._y + 1.0);

    return (1.0 - t) * math::RGBColor(0) + t * background;
}

raytracer::LightSample raytracer::getRayColor(const math::Ray &ray, const IShapesList &shapes, const ILightsList &lights,
    const RenderConfig &config, const unsigned depth, std::mt19937 &rng, const math::RGBColor &throughput)
{
    if (depth > config.maxDepth) {
        return {math::RGBColor(0), 1.0};
    }

    math::Intersect isect;
    if (!findClosestIntersection(ray, shapes, isect, true)) {
        return {getBackgroundColor(ray._dir, config.background) * throughput, 1.0};
    }

    // direct light
    math::RGBColor radiance(0);
    if (depth == 0) {
        const double ao = ambientOcclusion(isect, shapes, config.lighting.ambient.samples, rng);
        const math::RGBColor baseAmb = isect.object->getColorAt(isect.point) * config.lighting.ambient.coef;
        radiance = phongDirect(isect, -ray._dir, lights, shapes, config, rng);
        radiance = radiance - baseAmb + (baseAmb * ao);
    }

    // bsdf sampling
    const auto [bsdfWi, bsdfPdf, bsdfColor, bsdfSpecular] = isect.object->getMaterial().sample(-ray._dir, isect, rng);
    if (bsdfPdf < EPSILON) {
        return {radiance * throughput, 1.0};
    }

    // russian roulette
    math::RGBColor newThroughput = throughput * (bsdfColor / bsdfPdf);
    const double pContinue = std::min(1.0, newThroughput.maxComponent());

    if (material::getRandomDouble(rng) >= pContinue) {
        return {radiance * throughput, 1.0};
    }
    newThroughput /= pContinue;

    // recursive bounce
    const math::Ray nextRay = {isect.point + bsdfWi * EPSILON, bsdfWi};
    const LightSample next = getRayColor(nextRay, shapes, lights, config, depth + 1, rng, newThroughput);

    return {radiance * throughput + next.radiance, bsdfPdf, bsdfSpecular};
}
