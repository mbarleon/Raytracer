/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Color.cpp
*/

#include "../src/Elements/Scene/Material/Color.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <sstream>
#include <streambuf>

Test(RGBColor, default_constructor)
{
    const raytracer::RGBColor color;

    cr_assert_eq(color.r, 0.0, "Default red value should be 0.0");
    cr_assert_eq(color.g, 0.0, "Default green value should be 0.0");
    cr_assert_eq(color.b, 0.0, "Default blue value should be 0.0");
}

Test(RGBColor, parameterized_constructor)
{
    const raytracer::RGBColor color(0.5, 0.25, 0.75);

    cr_assert_eq(color.r, 0.5, "Red value should be set to 0.5");
    cr_assert_eq(color.g, 0.25, "Green value should be set to 0.25");
    cr_assert_eq(color.b, 0.75, "Blue value should be set to 0.75");
}

Test(RGBColor, addition_operator)
{
    const raytracer::RGBColor color1(0.5, 0.25, 0.75);
    const raytracer::RGBColor color2(0.25, 0.5, 0.25);
    const raytracer::RGBColor result = color1 + color2;

    cr_assert_eq(result.r, 0.75, "Red value should be 0.75");
    cr_assert_eq(result.g, 0.75, "Green value should be 0.75");
    cr_assert_eq(result.b, 1.0, "Blue value should be 1.0");
}

Test(RGBColor, multiplication_operator)
{
    const raytracer::RGBColor color1(0.5, 0.25, 0.75);
    const raytracer::RGBColor color2(0.25, 0.5, 0.25);
    const raytracer::RGBColor result = color1 * color2;

    cr_assert_eq(result.r, 0.125, "Red value should be 0.125");
    cr_assert_eq(result.g, 0.125, "Green value should be 0.125");
    cr_assert_eq(result.b, 0.1875, "Blue value should be 0.1875");
}

Test(RGBColor, scalar_multiplication_operator)
{
    const raytracer::RGBColor color(0.5, 0.25, 0.75);
    const double scalar = 2.0;
    const raytracer::RGBColor result = color * scalar;

    cr_assert_eq(result.r, 1.0, "Red value should be 1.0");
    cr_assert_eq(result.g, 0.5, "Green value should be 0.5");
    cr_assert_eq(result.b, 1.5, "Blue value should be 1.5");
}

Test(RGBColor, realign)
{
    raytracer::RGBColor color(1.5, -0.5, 2.0);

    color.realign(2.0, 255);
    cr_assert_eq(color.r, 255, "Red value should be clamped to 255");
    cr_assert_eq(color.g, 0, "Green value should be clamped to 0");
    cr_assert_eq(color.b, 255, "Blue value should be clamped to 255");
}

Test(RGBColor, print_color_operator)
{
    const raytracer::RGBColor color(1.3, -0.2, 2.0);
    const std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    std::cout << color << std::endl;
    cr_assert(buffer.str().find("1.3 -0.2 2") != std::string::npos, "Output should contain the color values");
    std::cout.rdbuf(old);
}
