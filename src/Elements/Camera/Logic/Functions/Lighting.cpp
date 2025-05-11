/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Lighting
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::sampleLightContribution(const math::Intersect &isect,
    const ILightsList &lights, const IShapesList &shapes)
{
    math::RGBColor totalRadiance(0);

    for (const auto &light : lights) {
        const material::BSDFSample lightSample = light->sample(isect.point);
        const math::Vector3D toLight = lightSample.direction.normalize();

        // test visibility
        const math::Ray shadowRay = { isect.point + toLight * EPSILON, toLight };

        math::Intersect shadowIsect;
        if (findClosestIntersection(shadowRay, shapes, shadowIsect, true) &&
        shadowIsect.distance + EPSILON < lightSample.pdf) {
            continue;
        }

        // evalute bsdf
        const raytracer::material::Material &material = isect.object->getMaterial();
        const math::RGBColor f = material.bsdf->evaluate(-shadowRay._dir, toLight, isect);

        if (!f.nearZero()) {
            totalRadiance += (f * lightSample.radiance);
        }
    }
    return totalRadiance;
}
