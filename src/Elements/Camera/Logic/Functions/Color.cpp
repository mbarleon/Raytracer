/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Background
*/

#include "../Pathtracer.hpp"
#include "../../../Scene/Materials/Utils/Utils.hpp"

math::RGBColor raytracer::getBackgroundColor(const math::Vector3D &v)
{
    const math::Vector3D dir = v.normalize();
    const auto t = 0.5 * (dir._y + 1.0);

    return (1.0 - t) * math::RGBColor(1) + t * math::RGBColor(0.3, 0.5, 1.0);
}

raytracer::LightSample raytracer::getRayColor(const math::Ray &ray,
    const IShapesList &shapes, const ILightsList &lights, const Render &render,
    unsigned depth, const math::RGBColor &throughput)
{
    if (depth > render.maxDepth) {
        return { math::RGBColor(0), 1.0 };
    }

    math::Intersect isect;
    if (!findClosestIntersection(ray, shapes, isect, true)) {
        return { getBackgroundColor(ray._dir) * throughput, 1.0 };
    }

    // direct light
    const math::Vector3D viewDir = -ray._dir.normalize();

    const double ao = ambientOcclusion(isect, shapes, /*samples=*/1);
    math::RGBColor radiance = phongDirect(isect, viewDir, lights, shapes, render);

    const math::RGBColor baseAmb = isect.object->getColor() * render.lighting.ambient;
    radiance = radiance - baseAmb + (baseAmb * ao);

    // bsdf sampling
    const auto bsdfS = isect.object->getMaterial().sample(-ray._dir, isect);
    if (bsdfS.pdf < EPSILON) {
        return { math::RGBColor(0), 1.0 };
    }

    // russian roulette
    math::RGBColor newThroughput = throughput * (bsdfS.radiance / bsdfS.pdf);
    const double pContinue = std::min(1.0, newThroughput.maxComponent());

    if (material::getRandomDouble() >= pContinue) {
        return { math::RGBColor(0), 1.0 };
    }
    newThroughput /= pContinue;

    // recursive bounce
    const math::Ray nextRay = { isect.point + bsdfS.direction * EPSILON, bsdfS.direction };
    const LightSample next = getRayColor(nextRay, shapes, lights, render, depth + 1, newThroughput);

    return { radiance * throughput + next.radiance, bsdfS.pdf };
}
