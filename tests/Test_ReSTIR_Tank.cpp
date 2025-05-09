/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_ReSTIR_Tank.cpp
*/

#include "../src/Elements/Render/Structs/ReSTIR_Tank.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

Test(restir_tank, default_constructor)
{
    const raytracer::restir_tank tank;

    cr_assert_eq(tank.count, 0);
    cr_assert_eq(tank.weightSum, 0.0);
}

Test(restir_tank, add)
{
    raytracer::restir_tank tank;
    raytracer::LightSample sample;
    std::mt19937 gen(42);

    sample.radiance = raytracer::RGBColor(1.0, 1.0, 1.0);
    sample.pdf = 1.0;
    tank.add(sample, 1.0, gen);
    cr_assert_eq(tank.count, 1);
    cr_assert_eq(tank.weightSum, 1.0);
    cr_assert_eq(tank.sample.radiance.r, 1.0);
    cr_assert(tank.sample.radiance.g == 1.0);
    cr_assert(tank.sample.radiance.b == 1.0);
}

Test(restir_tank, merge)
{
    raytracer::restir_tank tank1;
    raytracer::restir_tank tank2;
    raytracer::LightSample sample1;
    raytracer::LightSample sample2;
    std::mt19937 gen(42);

    sample1.radiance = raytracer::RGBColor(1.0, 0.0, 0.0);
    sample1.pdf = 1.0;
    tank1.add(sample1, 1.0, gen);
    sample2.radiance = raytracer::RGBColor(0.0, 1.0, 0.0);
    sample2.pdf = 1.0;
    tank2.add(sample2, 1.0, gen);
    tank1.merge(tank2, gen);
    cr_assert_eq(tank1.count, 2);
    cr_assert_eq(tank1.weightSum, 2.0);
}

Test(restir_tank, estimate)
{
    raytracer::restir_tank tank;
    raytracer::LightSample sample;
    std::mt19937 gen(42);

    sample.radiance = raytracer::RGBColor(1.0, 1.0, 1.0);
    sample.pdf = 1.0;
    tank.add(sample, 1.0, gen);

    const raytracer::RGBColor estimated = tank.estimate();

    cr_assert_eq(estimated.r, 1.0);
    cr_assert_eq(estimated.g, 1.0);
    cr_assert_eq(estimated.b, 1.0);
}
