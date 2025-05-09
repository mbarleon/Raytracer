/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Plane.cpp
*/

#include "../src/Elements/Scene/Shapes/Plane.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/test.h>

Test(Plane, PlaneConstructor)
{
    const raytracer::shape::Plane plane('X', 5.0);

    cr_assert_eq(plane.getPosition()._x, 5.0);
    cr_assert_eq(plane.getPosition()._y, 0.0);
    cr_assert_eq(plane.getPosition()._z, 0.0);
}

Test(Plane, PlaneGetNormalAtX)
{
    const raytracer::shape::Plane plane('X', 5.0);
    const math::Point3D point(0.0, 0.0, 0.0);

    cr_assert_eq(plane.getNormalAt(point)._x, 1.0);
    cr_assert_eq(plane.getNormalAt(point)._y, 0.0);
    cr_assert_eq(plane.getNormalAt(point)._z, 0.0);
}

Test(Plane, PlaneGetNormalAtY)
{
    const raytracer::shape::Plane plane('Y', 5.0);
    const math::Point3D point(0.0, 0.0, 0.0);

    cr_assert_eq(plane.getNormalAt(point)._x, 0.0);
    cr_assert_eq(plane.getNormalAt(point)._y, 1.0);
    cr_assert_eq(plane.getNormalAt(point)._z, 0.0);
}

Test(Plane, PlaneGetNormalAtZ)
{
    const raytracer::shape::Plane plane('Z', 5.0);
    const math::Point3D point(0.0, 0.0, 0.0);

    cr_assert_eq(plane.getNormalAt(point)._x, 0.0);
    cr_assert_eq(plane.getNormalAt(point)._y, 0.0);
    cr_assert_eq(plane.getNormalAt(point)._z, 1.0);
}

Test(Plane, PlaneGetPositionX)
{
    const raytracer::shape::Plane plane('X', 5.0);
    cr_assert_eq(plane.getPosition()._x, 5.0);
    cr_assert_eq(plane.getPosition()._y, 0.0);
    cr_assert_eq(plane.getPosition()._z, 0.0);
}

Test(Plane, PlaneGetPositionY)
{
    const raytracer::shape::Plane plane('Y', 5.0);

    cr_assert_eq(plane.getPosition()._x, 0.0);
    cr_assert_eq(plane.getPosition()._y, 5.0);
    cr_assert_eq(plane.getPosition()._z, 0.0);
}

Test(Plane, PlaneGetPositionZ)
{
    const raytracer::shape::Plane plane('Z', 5.0);

    cr_assert_eq(plane.getPosition()._x, 0.0);
    cr_assert_eq(plane.getPosition()._y, 0.0);
    cr_assert_eq(plane.getPosition()._z, 5.0);
}

Test(Plane, PlaneIntersectX)
{
    const raytracer::shape::Plane plane('X', 5.0);
    const math::Ray ray(math::Point3D(0.0, 0.0, 0.0), math::Vector3D(1.0, 0.0, 0.0));
    math::Point3D intersection;

    cr_assert_eq(plane.intersect(ray, intersection, false), true);
    cr_assert_eq(intersection._x, 5.0);
    cr_assert_eq(intersection._y, 0.0);
    cr_assert_eq(intersection._z, 0.0);
}

Test(Plane, PlaneIntersectY)
{
    const raytracer::shape::Plane plane('Y', 5.0);
    const math::Ray ray(math::Point3D(0.0, 0.0, 0.0), math::Vector3D(0.0, 1.0, 0.0));
    math::Point3D intersection;

    cr_assert_eq(plane.intersect(ray, intersection, false), true);
    cr_assert_eq(intersection._x, 0.0);
    cr_assert_eq(intersection._y, 5.0);
    cr_assert_eq(intersection._z, 0.0);
}

Test(Plane, PlaneIntersectZ)
{
    const raytracer::shape::Plane plane('Z', 5.0);
    const math::Ray ray(math::Point3D(0.0, 0.0, 0.0), math::Vector3D(0.0, 0.0, 1.0));
    math::Point3D intersection;

    cr_assert_eq(plane.intersect(ray, intersection, false), true);
    cr_assert_eq(intersection._x, 0.0);
    cr_assert_eq(intersection._y, 0.0);
    cr_assert_eq(intersection._z, 5.0);
}
