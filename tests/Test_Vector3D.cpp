/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Vector3D.cpp
*/

#include "../src/Maths/Vector3D.hpp"
#include <criterion/criterion.h>

Test(Vector3D, default_constructor)
{
    const math::Vector3D v;

    cr_assert_eq(v._x, 0);
    cr_assert_eq(v._y, 0);
    cr_assert_eq(v._z, 0);
}

Test(Vector3D, parameterized_constructor)
{
    const math::Vector3D v(1.0, 2.0, 3.0);

    cr_assert_eq(v._x, 1.0);
    cr_assert_eq(v._y, 2.0);
    cr_assert_eq(v._z, 3.0);
}

Test(Vector3D, addition_operator)
{
    const math::Vector3D a(1.0, 2.0, 3.0);
    const math::Vector3D b(4.0, 5.0, 6.0);
    const math::Vector3D c = a + b;

    cr_assert_eq(c._x, 5.0);
    cr_assert_eq(c._y, 7.0);
    cr_assert_eq(c._z, 9.0);
}

Test(Vector3D, subtraction_operator)
{
    const math::Vector3D a(5.0, 6.0, 7.0);
    const math::Vector3D b(1.0, 2.0, 3.0);
    const math::Vector3D c = a - b;

    cr_assert_eq(c._x, 4.0);
    cr_assert_eq(c._y, 4.0);
    cr_assert_eq(c._z, 4.0);
}

Test(Vector3D, scalar_multiplication)
{
    const math::Vector3D a(1.0, 2.0, 3.0);
    const math::Vector3D b = a * 2.0;

    cr_assert_eq(b._x, 2.0);
    cr_assert_eq(b._y, 4.0);
    cr_assert_eq(b._z, 6.0);
}

Test(Vector3D, scalar_division)
{
    const math::Vector3D a(2.0, 4.0, 6.0);
    const math::Vector3D b = a / 2.0;

    cr_assert_eq(b._x, 1.0);
    cr_assert_eq(b._y, 2.0);
    cr_assert_eq(b._z, 3.0);
}

Test(Vector3D, dot_product)
{
    const math::Vector3D a(1.0, 2.0, 3.0);
    const math::Vector3D b(4.0, -5.0, 6.0);
    const double dot = a.dot(b);

    cr_assert_eq(dot, 12.0);
}

Test(Vector3D, cross_product)
{
    const math::Vector3D a(1.0, 0.0, 0.0);
    const math::Vector3D b(0.0, 1.0, 0.0);
    const math::Vector3D c = a.cross(b);

    cr_assert_eq(c._x, 0.0);
    cr_assert_eq(c._y, 0.0);
    cr_assert_eq(c._z, 1.0);
}

Test(Vector3D, normalize)
{
    const math::Vector3D a(0.0, 3.0, 4.0);
    const math::Vector3D n = a.normalize();

    cr_assert_float_eq(n._x, 0.0, 1e-6);
    cr_assert_float_eq(n._y, 0.6, 1e-6);
    cr_assert_float_eq(n._z, 0.8, 1e-6);
}
