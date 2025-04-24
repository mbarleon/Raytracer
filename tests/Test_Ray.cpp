/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Ray.cpp
*/

#include "../src/Maths/Ray.hpp"
#include <criterion/criterion.h>

Test(ray, basic_ray)
{
    const math::Ray ray;

    cr_assert(ray._origin._x == 0);
}

Test(ray, test_ray_at)
{
    const math::Vector3D vec3(0.1, 2.0, 4.0);
    const math::Point3D p3(4.0, 5.0, 6.0);
    const math::Ray ray(p3, vec3);

    cr_assert(ray._origin._z == p3._z);
    cr_assert(ray.at(42)._z == 174);
    cr_assert(ray.at(2)._y == 9);
}
