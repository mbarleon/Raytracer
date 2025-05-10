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

math::RGBColor raytracer::getRayColor(const math::Ray &ray, const IShapesList &shapes,
    const Render &render, unsigned depth)
{
    math::Intersect intersect;

    if (findClosestIntersection(ray, shapes, intersect, true)) {
        if (depth >= render.maxDepth) {
            return math::RGBColor(0);
        }
        return intersect.object->getColor() * getRayColor(intersect.reflected, shapes,
            render, depth + 1);
    }
    return getBackgroundColor(ray._dir);
}
