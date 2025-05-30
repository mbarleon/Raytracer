/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Lighting
*/

#include "Pathtracer.hpp"

math::RGBColor raytracer::phongDirect(const math::Intersect &isect, const math::Vector3D &viewDir, const ILightsList &lights,
    const IShapesList &shapes, const RenderConfig &config, std::mt19937 &rng, raytracer::Tank &tank, const bool cullBackFaces)
{
    const auto *bsdf = isect.object->getMaterial().bsdf.get();
    const math::RGBColor baseColor = isect.object->getColorAt(isect.point);
    math::RGBColor Lo = baseColor * config.lighting.ambient.coef;

    for (const auto &light : lights) {
        const auto ls = light->sample(isect.point);
        const math::Vector3D L = ls.direction.normalize();
        const math::Ray shadow = {isect.point + isect.normal * EPSILON, L};

        if (math::Intersect occ; findClosestIntersection(shadow, shapes, occ, cullBackFaces) && occ.distance + EPSILON < ls.pdf) {
            LightSample sh { math::RGBColor(0.0), 1.0, false };
            tank.add(sh, config.lighting.extraShadow, rng);
            continue;
        }

        const double cosNL = std::max(0.0, isect.normal.dot(L));
        if (cosNL < EPSILON) {
            continue;
        }

        // diffuse
        const math::RGBColor f_diff = bsdf->evaluate(viewDir, L, isect, rng);
        Lo += f_diff * ls.radiance * cosNL * config.lighting.diffuse;

        // specular
        const math::Vector3D R = material::reflect(-L, isect.normal).normalize();
        const double RdotV = std::max(0.0, R.dot(viewDir));
        const double shininess = isect.object->getShininess();
        const math::RGBColor f_spec = baseColor * config.lighting.specular * std::pow(RdotV, shininess);

        Lo += f_spec * ls.radiance * cosNL;
    }
    return Lo;
}

double raytracer::ambientOcclusion(const math::Intersect &isect, const IShapesList &shapes, const unsigned int aoSamples,
    std::mt19937 &rng, const bool cullBackFaces)
{
    const unsigned int total = aoSamples * aoSamples;
    double occlusion = 0.0;

    const math::Vector3D T = isect.normal.orthogonal().cross(isect.normal).normalize();
    const math::Vector3D B = isect.normal.cross(T);

    std::uniform_real_distribution<double> u01(0.0, 1.0);

    for (unsigned int a = 0; a < total; ++a) {
        const unsigned int i = a % aoSamples;
        const unsigned int j = a / aoSamples;

        // jitter
        const double du = (i + u01(rng)) / static_cast<double>(aoSamples);
        const double dv = (j + u01(rng)) / static_cast<double>(aoSamples);

        // cos-weighted sample
        const double r = std::sqrt(du);
        const double theta = 2.0 * M_PI * dv;
        const double x = r * std::cos(theta);
        const double y = r * std::sin(theta);
        const double z = std::sqrt(std::max(0.0, 1.0 - du));

        const math::Vector3D dir = (T * x + B * y + isect.normal * z).normalize();
        const math::Ray ray = {isect.point + dir * EPSILON, dir};

        if (math::Intersect tmp; findClosestIntersection(ray, shapes, tmp, cullBackFaces)) {
            const double d = tmp.distance;
            const double w = std::exp(-d / tmp.object->getAOMaxDistance());
            occlusion += w;
        }
    }
    return 1.0 - (occlusion / static_cast<double>(total));
}
