/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Tank.cpp
*/

#include "../src/Elements/Render/ReSTIR/Tank.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

Test(Tank, default_constructor)
{
    const raytracer::Tank tank;

    cr_assert_eq(tank.count, 0);
    cr_assert_eq(tank.weightSum, 0.0);
}

Test(Tank, add)
{
    raytracer::Tank tank;
    raytracer::LightSample sample;
    std::mt19937 gen(42);

    sample.radiance = math::RGBColor(1.0, 1.0, 1.0);
    sample.pdf = 1.0;
    tank.add(sample, 1.0, gen);
    cr_assert_eq(tank.count, 1);
    cr_assert_eq(tank.weightSum, 1.0);
    cr_assert_eq(tank.sample.radiance._x, 1.0);
    cr_assert(tank.sample.radiance._y == 1.0);
    cr_assert(tank.sample.radiance._z == 1.0);
}

Test(Tank, merge)
{
    raytracer::Tank tank1;
    raytracer::Tank tank2;
    raytracer::LightSample sample1;
    raytracer::LightSample sample2;
    std::mt19937 gen(42);

    sample1.radiance = math::RGBColor(1.0, 0.0, 0.0);
    sample1.pdf = 1.0;
    tank1.add(sample1, 1.0, gen);
    sample2.radiance = math::RGBColor(0.0, 1.0, 0.0);
    sample2.pdf = 1.0;
    tank2.add(sample2, 1.0, gen);
    tank1.merge(tank2, gen);
    cr_assert_eq(tank1.count, 2);
    cr_assert_eq(tank1.weightSum, 2.0);
}

Test(Tank, estimate)
{
    raytracer::Tank tank;
    raytracer::LightSample sample;
    std::mt19937 gen(42);

    sample.radiance = math::RGBColor(1.0, 1.0, 1.0);
    sample.pdf = 1.0;
    tank.add(sample, 1.0, gen);

    const math::RGBColor estimated = tank.estimate();

    cr_assert_eq(estimated._x, 1.0);
    cr_assert_eq(estimated._y, 1.0);
    cr_assert_eq(estimated._z, 1.0);
}
