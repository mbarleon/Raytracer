/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Background
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::getBackgroundColor(const math::Vector3D &v)
{
    const math::Vector3D dir = v.normalize();
    const auto t = 0.5 * (dir._y + 1.0);

    return (1.0 - t) * math::RGBColor(1) + t * math::RGBColor(0.3, 0.5, 1.0);
}

raytracer::LightSample raytracer::getRayColor(const math::Ray &ray, const IShapesList &shapes,
    const ILightsList &lights, const Render &render, unsigned depth)
{
    raytracer::LightSample sample = { math::RGBColor(0), EPSILON };

    if (depth > render.maxDepth)
        return sample;

    math::Intersect intersect;
    if (!findClosestIntersection(ray, shapes, intersect, true)) {
        sample.radiance = getBackgroundColor(ray._dir);
        sample.pdf = 1.0;
        return sample;
    }

    const math::Vector3D V = -ray._dir;
    const auto &mat = intersect.object->getMaterial();
    const auto bsdfSample = mat.bsdf->sample(V, intersect);

    if (bsdfSample.pdf < EPSILON)
        return sample;

    const math::Ray nextRay = {intersect.point + bsdfSample.direction * EPSILON, bsdfSample.direction};
    const LightSample next = getRayColor(nextRay, shapes, lights, render, depth + 1);

    sample.radiance = bsdfSample.value * next.radiance;
    sample.pdf = bsdfSample.pdf;
    return sample;
}
