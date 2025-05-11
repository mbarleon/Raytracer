/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Reflect
*/

#include "../Pathtracer.hpp"

math::Vector3D raytracer::getReflectedVector(const math::Vector3D &I, const math::Vector3D &N)
{
    return I - N * (2.0 * I.dot(N));
}

math::Vector3D raytracer::getRandomReflectUnitSphere()
{
    while (1) {
        const math::Vector3D p = getRandomVector(-1.0, 1.0);
        const double lensq = p.lengthSquared();

        if (EPSILON < lensq && lensq <= 1) {
            return p / sqrt(lensq);
        }
    }
}

math::Vector3D raytracer::reflectHemisphere(const math::Vector3D &normal)
{
    const math::Vector3D p = getRandomReflectUnitSphere();

    return (p.dot(normal) > 0.0) ? p : -p;
}
