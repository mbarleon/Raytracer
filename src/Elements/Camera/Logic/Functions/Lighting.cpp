/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Lighting
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::computeDirectLight(const math::Intersect &intersect,
    const math::Vector3D &V, const Material &mat, const IShapesList &shapes,
    const ILightsList &lights, const Render &render)
{
    math::RGBColor Lo(0);

    for (const auto &light : lights) {
        // distance
        const math::Vector3D toL = light->getPosition() - intersect.point;
        const double dist2 = toL.lengthSquared();
        const double dist = std::sqrt(dist2);
        const math::Vector3D L = toL / dist;

        // shadow ray
        math::Ray shadowRay;
        shadowRay._origin = intersect.point + intersect.normal * EPSILON;
        shadowRay._dir = L;

        math::Intersect shadowIsect;
        if (findClosestIntersection(shadowRay, shapes, shadowIsect, true)
        && shadowIsect.distance < dist)
            continue;

        const double cosNL = std::max(0.0, intersect.normal.dot(L));

        // Diffuse (Lambertien)
        Lo += intersect.object->getColor() * light->getColor() *
            render.lighting.diffuse * cosNL / dist2;

        // Spéculaire (Phong)
        const math::Vector3D R = getReflectedVector(-L, intersect.normal).normalize();
        const double cosRV = std::max(0.0, R.dot(V));
        const double phong = std::pow(cosRV, mat.shininess);

        Lo += light->getColor() * render.lighting.specular * phong / dist2;
    }
    return Lo;
}
