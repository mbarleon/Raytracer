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
            if (dist < EPSILON) {
                continue;
            }
            if (dist < distMin) {
                distMin = dist;
                hit = true;
                intersect.object = shape;
                intersect.point = intersectPoint;
                intersect.distance = dist;
            }
        }
    }
    if (hit) {
        math::Vector3D direction;

        intersect.normal = intersect.object->getNormalAt(intersectPoint);
        if (intersect.object->getMaterial()->reflectivity > 0.0) {
            direction = getReflectedVector(ray._dir, intersect.normal);
        } else {
            direction = intersect.normal + getRandomReflectUnitSphere().normalize();
        }
        intersect.reflected = {intersect.point + intersect.normal * EPSILON, direction};
    }
    return hit;
}
