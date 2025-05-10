/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Reflect
*/

#include "../Pathtracer.hpp"

math::Vector3D raytracer::getRandomReflectUnitSphere()
{
    math::Vector3D inUnitSphere = getRandomVector(-1.0, 1.0);

    while (inUnitSphere.lengthSquared() >= 1.0) {
        inUnitSphere = getRandomVector(-1.0, 1.0);
    }
    return inUnitSphere;
}

math::Vector3D raytracer::getRandomReflect(const math::Vector3D &normal)
{
    const math::Vector3D inUnitSphere = getRandomReflectUnitSphere().normalize();

    if (inUnitSphere.dot(normal) > 0.0) {
        return inUnitSphere;
    }
    return -inUnitSphere;
}
