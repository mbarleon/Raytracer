/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Sphere.cpp
*/

#include "../src/Shapes/Sphere.hpp"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(sphere, intersection_exists)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 0, -5), math::Vector3D(0, 0, 1));
    const bool result = sphere.intersect(ray);

    cr_assert_not(!result, "math::Ray should intersect sphere");
}

Test(sphere, no_intersection)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 5, -5), math::Vector3D(0, 0, 1));
    const bool result = sphere.intersect(ray);

    cr_assert_not(result, "math::Ray should miss sphere");
}

Test(sphere, tangent_intersection)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 1, -5), math::Vector3D(0, 0, 1));
    const bool result = sphere.intersect(ray);

    cr_assert_not(!result, "math::Ray should be tangent to sphere");
}

Test(sphere, inside_sphere)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 0, 0), math::Vector3D(0, 0, 1));
    const bool result = sphere.intersect(ray);

    cr_assert_not(!result, "math::Ray starting inside sphere should intersect");
}
