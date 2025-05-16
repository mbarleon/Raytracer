/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Intersect
*/

#include "Pathtracer.hpp"
#include <limits>

bool raytracer::findClosestIntersection(const math::Ray &ray, const IShapesList &shapes, math::Intersect &intersect,
    const bool cullBackFaces)
{
    double distMin = std::numeric_limits<double>::infinity();
    math::Point3D intersectPoint;
    bool hit = false;

    for (const auto &shape : shapes) {
        if (shape != nullptr && shape->intersect(ray, intersectPoint, cullBackFaces)) {
            const double dist = (intersectPoint - ray._origin).length();
            if (dist < 0.001) {
                continue;
            }
            if (dist < distMin) {
                distMin = dist;
                hit = true;
                intersect.object = shape.get();
                intersect.point = intersectPoint;
                intersect.distance = dist;
            }
        }
    }
    if (hit) {
        intersect.normal = intersect.object->getNormalAt(intersectPoint);
    }
    return hit;
}
