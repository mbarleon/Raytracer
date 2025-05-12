/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Lighting
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::phongDirect(const math::Intersect &isect,
    const math::Vector3D &viewDir, const ILightsList &lights,
    const IShapesList &shapes, const Render &render, std::mt19937 &rng)
{
    const auto *bsdf = isect.object->getMaterial().bsdf.get();
    const math::RGBColor baseColor = isect.object->getColor();
    math::RGBColor Lo = baseColor * render.lighting.ambient;

    for (const auto &light : lights) {
        const auto ls = light->sample(isect.point);
        const math::Vector3D L = ls.direction.normalize();
        const math::Ray shadow = { isect.point + isect.normal * EPSILON, L };

        math::Intersect occ;
        if (findClosestIntersection(shadow, shapes, occ, true) &&
        occ.distance + EPSILON < ls.pdf) {
            continue;
        }

        const double cosNL = std::max(0.0, isect.normal.dot(L));
        if (cosNL < EPSILON) {
            continue;
        }

        // diffuse
        const math::RGBColor f_diff = bsdf->evaluate(viewDir, L, isect, rng);
        Lo += f_diff * ls.radiance * cosNL * render.lighting.diffuse;

        // specular
        const math::Vector3D R = material::reflect(-L, isect.normal).normalize();
        const double RdotV = std::max(0.0, R.dot(viewDir));
        const double shininess = 100.0; // mat.getShininess()
        const math::RGBColor f_spec = baseColor * render.lighting.specular *
            std::pow(RdotV, shininess);

        Lo += f_spec * ls.radiance * cosNL;
    }
    return Lo;
}

double raytracer::ambientOcclusion(const math::Intersect &isect,
    const IShapesList &shapes, int N, std::mt19937 &rng)
{
    const int total = N * N;
    const double maxDistance = 1.0; // shape -> getAOMaxDistance
    double occlusion = 0.0;

    const math::Vector3D T = isect.normal.orthonormal().cross(isect.normal).normalize();
    const math::Vector3D B = isect.normal.cross(T);

    std::uniform_real_distribution<double> u01(0.0, 1.0);

    for (int a = 0; a < total; ++a) {
        const int i = a % N;
        const int j = a / N;

        // jitter
        const double du = (i + u01(rng)) / static_cast<double>(N);
        const double dv = (j + u01(rng)) / static_cast<double>(N);

        // cos-weighted sample
        const double r = std::sqrt(du);
        const double theta = 2.0 * M_PI * dv;
        const double x = r * std::cos(theta);
        const double y = r * std::sin(theta);
        const double z = std::sqrt(std::max(0.0, 1.0 - du));

        const math::Vector3D dir = (T * x + B * y + isect.normal * z).normalize();

        const math::Ray ray = { isect.point + dir * EPSILON, dir };
        math::Intersect tmp;
        if (findClosestIntersection(ray, shapes, tmp, true)) {
            const double d = tmp.distance;
            const double w = std::exp(-d / maxDistance);
            occlusion += w;
        }
    }
    return 1.0 - (occlusion / static_cast<double>(total));
}
