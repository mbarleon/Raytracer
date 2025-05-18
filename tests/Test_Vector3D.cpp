/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Vector3D.cpp
*/

#include "../src/Maths/Vector3D.hpp"
#include <criterion/criterion.h>
#include <iostream>
#include <math.h>
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

Test(Vector3D, test_vector3D_double_all)
{
    const math::Vector3D vec3(1);

    cr_assert_eq(vec3._x, 1);
    cr_assert_eq(vec3._y, 1);
    cr_assert_eq(vec3._z, 1);
}

Test(Vector3D, test_vector3D_triple)
{
    const math::Vector3D vec3(1, 2, 3);

    cr_assert_eq(vec3._x, 1);
    cr_assert_eq(vec3._y, 2);
    cr_assert_eq(vec3._z, 3);
}

Test(Vector3D, length_squared)
{
    const math::Vector3D vec3(1, 2, 3);

    cr_assert_eq(vec3.lengthSquared(), 14);
}

Test(Vector3D, negative)
{
    const math::Vector3D vec3(1, 2, 3);
    const math::Vector3D neg = -vec3;

    cr_assert_eq(neg._x, -1);
    cr_assert_eq(neg._y, -2);
    cr_assert_eq(neg._z, -3);
}

Test(Vector3D, orthogonal_x_dominant)
{
    const math::Vector3D vec(5, 2, 3);
    const math::Vector3D ortho = vec.orthogonal();

    cr_assert_float_eq(vec.dot(ortho), 0.0, 1e-6);
    cr_assert_float_eq(ortho._x, -2.0, 1e-6);
    cr_assert_float_eq(ortho._y, 5.0, 1e-6);
    cr_assert_float_eq(ortho._z, 0.0, 1e-6);
}

Test(Vector3D, orthogonal_z_dominant)
{
    const math::Vector3D vec(1, 2, 5);
    const math::Vector3D ortho = vec.orthogonal();

    cr_assert_float_eq(vec.dot(ortho), 0.0, 1e-6);
    cr_assert_float_eq(ortho._x, 0.0, 1e-6);
    cr_assert_float_eq(ortho._y, -5.0, 1e-6);
    cr_assert_float_eq(ortho._z, 2.0, 1e-6);
}

Test(Vector3D, orthogonal_edge_case)
{
    const math::Vector3D vec(3, 1, -3);
    const math::Vector3D ortho = vec.orthogonal();

    cr_assert_eq(vec.dot(ortho), 0.0);
    cr_assert_eq(ortho._x, 0.0);
    cr_assert_eq(ortho._y, 3.0);
    cr_assert_eq(ortho._z, 1.0);
}

Test(Vector3D, max_component_x)
{
    const math::Vector3D vec1(5.0, 2.0, 3.0);

    cr_assert_eq(vec1.maxComponent(), 5.0);
}

Test(Vector3D, max_component_y)
{
    const math::Vector3D vec2(1.0, 4.0, 3.0);

    cr_assert_eq(vec2.maxComponent(), 4.0);
}

Test(Vector3D, max_component_z)
{
    const math::Vector3D vec3(1.0, 2.0, 5.0);

    cr_assert_eq(vec3.maxComponent(), 5.0);
}

Test(Vector3D, max_component_equal_values)
{
    const math::Vector3D vec4(2.0, 2.0, 1.0);
    const math::Vector3D vec5(1.0, 3.0, 3.0);
    const math::Vector3D vec6(4.0, 4.0, 4.0);

    cr_assert_eq(vec4.maxComponent(), 1.0);
    cr_assert_eq(vec5.maxComponent(), 3.0);
    cr_assert_eq(vec6.maxComponent(), 4.0);
}

Test(Vector3D, near_zero)
{
    const math::Vector3D vec3(0.000000001, 0.000000001, 0.000000001);
    const bool isNearZero = vec3.nearZero();

    cr_assert_eq(isNearZero, true);
}

Test(Vector3D, luminance)
{
    const math::Vector3D vec3(0.5, 0.5, 0.5);
    const double lum = vec3.luminance();

    cr_assert_float_eq(lum, 0.5, 1e-6);
}

Test(Vector3D, apply_rotation_no_rotation)
{
    const math::Vector3D dir(1, 0, 0);
    const math::Vector3D rot(0, 0, 0);
    const math::Vector3D result = math::Vector3D().applyRotation(dir, rot);

    cr_assert_eq(result._x, 1);
    cr_assert_eq(result._y, 0);
    cr_assert_eq(result._z, 0);
}

Test(Vector3D, apply_rotation_90deg_x)
{
    const math::Vector3D dir(1, 0, 0);
    const math::Vector3D rot(M_PI / 2, 0, 0);
    const math::Vector3D result = math::Vector3D().applyRotation(dir, rot);

    cr_assert_eq(result._x, 1);
    cr_assert_eq(result._y, 0);
    cr_assert_eq(result._z, 0);
}

Test(Vector3D, apply_rotation_90deg_y)
{
    const math::Vector3D dir(1, 0, 0);
    const math::Vector3D rot(0, M_PI / 2, 0);
    const math::Vector3D result = math::Vector3D().applyRotation(dir, rot);

    cr_assert_float_eq(result._x, 6.12323e-17, 1e-6);
    cr_assert_eq(result._y, 0);
    cr_assert_eq(result._z, -1);
}

Test(Vector3D, apply_rotation_90deg_z)
{
    const math::Vector3D dir(1, 0, 0);
    const math::Vector3D rot(0, 0, M_PI / 2);
    const math::Vector3D result = math::Vector3D().applyRotation(dir, rot);

    cr_assert_float_eq(result._x, 6.12323e-17, 1e-6);
    cr_assert_eq(result._y, 1);
    cr_assert_eq(result._z, 0);
}

Test(Vector3D, apply_rotation_180deg_y)
{
    const math::Vector3D dir(1, 0, 0);
    const math::Vector3D rot(0, M_PI, 0);
    const math::Vector3D result = math::Vector3D().applyRotation(dir, rot);

    cr_assert_eq(result._x, -1);
    cr_assert_eq(result._y, 0);
    cr_assert_float_eq(result._z, -1.22465e-16, 1e-6);
}

Test(Vector3D, apply_rotation_combined)
{
    const math::Vector3D dir(1, 0, 0);
    const math::Vector3D rot(M_PI / 2, M_PI / 2, 0);
    const math::Vector3D result = math::Vector3D().applyRotation(dir, rot);

    cr_assert_float_eq(result._x, 6.12323e-17, 1e-6);
    cr_assert_eq(result._y, 0);
    cr_assert_eq(result._z, -1);
}
