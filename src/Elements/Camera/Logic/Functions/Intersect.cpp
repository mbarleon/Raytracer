/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Intersect
*/

#include "../Pathtracer.hpp"
#include <limits>

bool raytracer::findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect, const bool cullBackFaces)
{
    double distMin = std::numeric_limits<double>::infinity();
    math::Point3D intersectPoint;
    bool hit = false;

    for (const auto &shape : shapes) {
        if (shape->intersect(ray, intersectPoint, cullBackFaces)) {
            const double dist = (intersectPoint - ray._origin).length();
            if (dist < distMin) {
                distMin = dist;
                hit = true;
                intersect.object = shape;
                intersect.point = intersectPoint;
                intersect.normal = shape->getNormalAt(intersectPoint);
                intersect.distance = dist;
            }
        }
    }
    return hit;
}
