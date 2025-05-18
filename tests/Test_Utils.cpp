/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Utils.cpp
*/

#include "../src/Elements/Scene/Materials/Utils/Utils.hpp"
#include <criterion/criterion.h>
#include <random>

Test(Utils, reflect_basic_case)
{
    math::Vector3D I(1, -1, 0);
    math::Vector3D N(0, 1, 0);
    const math::Vector3D result = raytracer::material::reflect(I, N);

    cr_assert_float_eq(result._x, 1.0, 1e-6);
    cr_assert_float_eq(result._y, 1.0, 1e-6);
    cr_assert_float_eq(result._z, 0.0, 1e-6);
}

Test(Utils, reflect_perpendicular)
{
    math::Vector3D I(0, -1, 0);
    math::Vector3D N(0, 1, 0);
    const math::Vector3D result = raytracer::material::reflect(I, N);

    cr_assert_float_eq(result._x, 0.0, 1e-6);
    cr_assert_float_eq(result._y, 1.0, 1e-6);
    cr_assert_float_eq(result._z, 0.0, 1e-6);
}

Test(Utils, refract_normal_incidence)
{
    math::Vector3D I(0, -1, 0);
    math::Vector3D N(0, 1, 0);
    const math::Vector3D result = raytracer::material::refract(I, N, 1.5);

    cr_assert_float_eq(result._x, 0.0, 1e-6);
    cr_assert_float_eq(result._y, -1.0, 1e-6);
    cr_assert_float_eq(result._z, 0.0, 1e-6);
}

Test(Utils, refract_critical_angle)
{
    math::Vector3D I(-sqrt(2) / 2, -sqrt(2) / 2, 0);
    math::Vector3D N(0, 1, 0);
    const math::Vector3D result = raytracer::material::refract(I, N, 1.0 / 1.5);

    cr_assert_float_eq(result._x, -0.4714405, 1e-1);
    cr_assert_float_eq(result._y, -0.881917, 1e-1);
    cr_assert_float_eq(result._z, 0.0, 1e-1);
}

Test(Utils, reflectance_normal_incidence)
{
    const double result = raytracer::material::reflectance(1.0, 1.0, 1.5);

    cr_assert_float_eq(result, 0.04, 1e-6);
}

Test(Utils, reflectance_grazing_angle)
{
    const double result = raytracer::material::reflectance(0.0, 1.0, 1.5);

    cr_assert_float_eq(result, 1.0, 1e-6);
}

using u8 = unsigned char;

Test(Utils, cosineHemisphere_basic)
{
    std::mt19937 rng(42);
    const math::Vector3D N(0, 1, 0);

    for (u8 i = 0; i < 100; ++i) {
        const math::Vector3D result = raytracer::material::cosineHemisphere(N, rng);

        cr_assert_float_eq(result.length(), 1.0, 1e-6);
        cr_assert(result.dot(N) >= 0.0);
    }
}

Test(Utils, cosineHemisphere_direction)
{
    std::mt19937 rng(42);
    math::Vector3D N(1, 1, 0);
    N = N.normalize();

    for (u8 i = 0; i < 100; ++i) {
        const math::Vector3D result = raytracer::material::cosineHemisphere(N, rng);
        cr_assert(result.dot(N) >= 0.0);
    }
}
