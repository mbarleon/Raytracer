/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Sphere.cpp
*/

#include "../src/Elements/Scene/Shapes/Sphere/Sphere.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/new/assert.h>
#include <memory>

Test(sphere, intersection_exists)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray = {math::Point3D(0, 0, -5), math::Vector3D(0, 0, 1)};
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(!result, "math::Ray should intersect sphere");
}

Test(sphere, no_intersection)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray = {math::Point3D(0, 5, -5), math::Vector3D(0, 0, 1)};
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(result, "math::Ray should miss sphere");
}

Test(sphere, tangent_intersection)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray = {math::Point3D(0, 1, -5), math::Vector3D(0, 0, 1)};
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(!result, "math::Ray should be tangent to sphere");
}

Test(sphere, inside_sphere)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray = {math::Point3D(0, 0, 0), math::Vector3D(0, 0, 1)};
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(!result, "math::Ray starting inside sphere should intersect");
}

Test(sphere, set_color)
{
    raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::RGBColor color(0.5, 0.5, 0.5);

    sphere.setColor(color);
    cr_assert_eq(color._x, sphere.getColor()._x, "Sphere color should be set correctly (r)");
    cr_assert_eq(color._y, sphere.getColor()._y, "Sphere color should be set correctly (g)");
    cr_assert_eq(color._z, sphere.getColor()._z, "Sphere color should be set correctly (b)");
}

Test(sphere, get_position)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);

    cr_assert_eq(sphere.getPosition()._x, 0.0, "Sphere position should be (0, 0, 0) (x)");
    cr_assert_eq(sphere.getPosition()._y, 0.0, "Sphere position should be (0, 0, 0) (y)");
    cr_assert_eq(sphere.getPosition()._z, 0.0, "Sphere position should be (0, 0, 0) (z)");
}

Test(sphere, get_normal_at)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Point3D point(1, 0, 0);
    const math::Vector3D normal = sphere.getNormalAt(point);
    cr_assert_eq(normal._x, 1.0, "Sphere normal at (1, 0, 0) should be (1, 0, 0) (x)");
    cr_assert_eq(normal._y, 0.0, "Sphere normal at (1, 0, 0) should be (1, 0, 0) (y)");
    cr_assert_eq(normal._z, 0.0, "Sphere normal at (1, 0, 0) should be (1, 0, 0) (z)");
}
