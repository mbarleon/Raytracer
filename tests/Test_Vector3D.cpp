/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Vector3D.cpp
*/

#include "../src/Maths/Vector3D.hpp"
#include <criterion/criterion.h>
#include <sstream>
#include <streambuf>

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

#include <iostream>
Test(Vector3D, general_operations)
{
    const math::Vector3D a(1.0, 2.0, 3.0);
    const math::Vector3D b(4.0, 5.0, 6.0);
    const math::Vector3D e(3.0, 3.0, 3.0);
    math::Vector3D c = a + b;

    cr_assert((c += a)._z == 12);
    cr_assert((c -= a)._y == 7);
    cr_assert((c *= a)._x == 5);
    cr_assert((c /= a)._z == 9);
    c = c * a;
    c = c / a;
    c = c * 2;
    c = c / 2;
    c *= 2;
    c /= 2;
    -c;
    cr_assert_eq(b - a, e);
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

Test(Vector3D, normalize_null_len)
{
    const math::Vector3D a(0.0, 0.0, 0.0);
    const math::Vector3D n = a.normalize();

    cr_assert_eq(n._x, 0.0);
    cr_assert_eq(n._y, 0.0);
    cr_assert_eq(n._z, 0.0);
}

Test(Vector3D, equal)
{
    const math::Vector3D a(0.0, 3.0, 4.0);
    const math::Vector3D b(0.0, 3.0, 4.0);
    const math::Vector3D c(1.0, 3.0, 4.0);
    const math::Vector3D d(0.0, 1.0, 4.0);
    const math::Vector3D e(0.0, 3.0, 1.0);

    cr_assert(a == b);
    cr_assert_eq(a == c, false);
    cr_assert_eq(a == d, false);
    cr_assert_eq(a == e, false);
}

Test(Vector3D, different)
{
    const math::Vector3D a(0.0, 3.0, 4.0);
    const math::Vector3D b(0.0, 3.0, 4.0);
    const math::Vector3D c(1.0, 3.0, 4.0);
    const math::Vector3D d(0.0, 1.0, 4.0);
    const math::Vector3D e(0.0, 3.0, 1.0);

    cr_assert(a != c);
    cr_assert(a != d);
    cr_assert(a != e);
    cr_assert_eq(a != b, false);
}

Test(Vector3D, test_vector3d_print)
{
    const math::Vector3D vec3(1.0, 2.0, 3.0);
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    std::cout << vec3 << std::endl;
    std::cout.rdbuf(old);
    cr_assert(buffer.str().find("1") != std::string::npos);
    cr_assert(buffer.str().find("2") != std::string::npos);
    cr_assert(buffer.str().find("3") != std::string::npos);
}
