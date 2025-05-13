/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Color.cpp
*/

#include "../src/Maths/Vector3D.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <sstream>
#include <streambuf>
#include <iostream>

Test(RGBColor, default_constructor)
{
    const math::RGBColor color;

    cr_assert_eq(color._x, 0.0, "Default red value should be 0.0");
    cr_assert_eq(color._y, 0.0, "Default green value should be 0.0");
    cr_assert_eq(color._z, 0.0, "Default blue value should be 0.0");
}

Test(RGBColor, parameterized_constructor)
{
    const math::RGBColor color(0.5, 0.25, 0.75);

    cr_assert_eq(color._x, 0.5, "Red value should be set to 0.5");
    cr_assert_eq(color._y, 0.25, "Green value should be set to 0.25");
    cr_assert_eq(color._z, 0.75, "Blue value should be set to 0.75");
}

Test(RGBColor, addition_operator)
{
    const math::RGBColor color1(0.5, 0.25, 0.75);
    const math::RGBColor color2(0.25, 0.5, 0.25);
    const math::RGBColor result = color1 + color2;

    cr_assert_eq(result._x, 0.75, "Red value should be 0.75");
    cr_assert_eq(result._y, 0.75, "Green value should be 0.75");
    cr_assert_eq(result._z, 1.0, "Blue value should be 1.0");
}

Test(RGBColor, multiplication_operator)
{
    const math::RGBColor color1(0.5, 0.25, 0.75);
    const math::RGBColor color2(0.25, 0.5, 0.25);
    const math::RGBColor result = color1 * color2;

    cr_assert_eq(result._x, 0.125, "Red value should be 0.125");
    cr_assert_eq(result._y, 0.125, "Green value should be 0.125");
    cr_assert_eq(result._z, 0.1875, "Blue value should be 0.1875");
}

Test(RGBColor, scalar_multiplication_operator)
{
    const math::RGBColor color(0.5, 0.25, 0.75);
    const double scalar = 2.0;
    const math::RGBColor result = color * scalar;

    cr_assert_eq(result._x, 1.0, "Red value should be 1.0");
    cr_assert_eq(result._y, 0.5, "Green value should be 0.5");
    cr_assert_eq(result._z, 1.5, "Blue value should be 1.5");
}

