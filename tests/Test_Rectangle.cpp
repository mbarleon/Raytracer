/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Rectangle.cpp
*/

#include "../src/Elements/Scene/Shapes/Rectangle.hpp"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(rectangle, intersection_center)
{
    const raytracer::shape::Rectangle rect(math::Point3D(0, 0, 0), math::Vector3D(2, 0, 0), math::Vector3D(0, 2, 0));
    const math::Ray ray(math::Point3D(0, 0, -5), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = rect.intersect(ray, intPoint);

    cr_assert_not(!result, "Ray should intersect rectangle center");
}

Test(rectangle, intersection_edge)
{
    const raytracer::shape::Rectangle rect(math::Point3D(0, 0, 0), math::Vector3D(2, 0, 0), math::Vector3D(0, 2, 0));
    const math::Ray ray(math::Point3D(1, 1, -5), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = rect.intersect(ray, intPoint);

    cr_assert_not(!result, "Ray should intersect rectangle edge");
}

Test(rectangle, no_intersection_outside)
{
    const raytracer::shape::Rectangle rect(math::Point3D(0, 0, 0), math::Vector3D(2, 0, 0), math::Vector3D(0, 2, 0));
    const math::Ray ray(math::Point3D(3, 3, -5), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = rect.intersect(ray, intPoint);

    cr_assert_not(result, "Ray should miss rectangle");
}

Test(rectangle, parallel_ray)
{
    const raytracer::shape::Rectangle rect(math::Point3D(0, 0, 0), math::Vector3D(2, 0, 0), math::Vector3D(0, 2, 0));
    const math::Ray ray(math::Point3D(0, 0, -5), math::Vector3D(1, 0, 0));
    math::Point3D intPoint;
    const bool result = rect.intersect(ray, intPoint);

    cr_assert_not(result, "Parallel ray should not intersect");
}
