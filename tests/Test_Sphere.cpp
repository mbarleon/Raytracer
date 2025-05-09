/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_Sphere.cpp
*/

#include "../src/Elements/Scene/Shapes/Sphere.hpp"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/new/assert.h>
#include <memory>

Test(sphere, intersection_exists)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 0, -5), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(!result, "math::Ray should intersect sphere");
}

Test(sphere, no_intersection)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 5, -5), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(result, "math::Ray should miss sphere");
}

Test(sphere, tangent_intersection)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 1, -5), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(!result, "math::Ray should be tangent to sphere");
}

Test(sphere, inside_sphere)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Ray ray(math::Point3D(0, 0, 0), math::Vector3D(0, 0, 1));
    math::Point3D intPoint;
    const bool result = sphere.intersect(ray, intPoint, false);

    cr_assert_not(!result, "math::Ray starting inside sphere should intersect");
}

Test(sphere, set_color)
{
    raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const raytracer::RGBColor color(0.5, 0.5, 0.5);

    sphere.setColor(color);
    cr_assert_eq(color.r, sphere.getColor().r, "Sphere color should be set correctly (r)");
    cr_assert_eq(color.g, sphere.getColor().g, "Sphere color should be set correctly (g)");
    cr_assert_eq(color.b, sphere.getColor().b, "Sphere color should be set correctly (b)");
}

Test(sphere, set_material)
{
    raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    std::shared_ptr<raytracer::Material> material = std::make_shared<raytracer::Material>();

    material.get()->emissiveIntensity = 0.1;
    material.get()->reflectivity = 0.2;
    material.get()->refractiveIndex = 0.3;
    material.get()->shininess = 0.4;

    sphere.setMaterial(material);

    cr_assert_eq(sphere.getMaterial()->emissiveIntensity, material.get()->emissiveIntensity,
        "Sphere material should be set correctly (emissiveIntensity)");
    cr_assert_eq(sphere.getMaterial()->reflectivity, material.get()->reflectivity,
        "Sphere material should be set correctly (reflectivity)");
    cr_assert_eq(sphere.getMaterial()->refractiveIndex, material.get()->refractiveIndex,
        "Sphere material should be set correctly (refractiveIndex)");
    cr_assert_eq(sphere.getMaterial()->shininess, material.get()->shininess,
        "Sphere material should be set correctly (shininess)");
}

Test(sphere, get_position)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);

    cr_assert_eq(sphere.getPosition()._x, 0.0, "Sphere position should be (0, 0, 0) (x)");
    cr_assert_eq(sphere.getPosition()._y, 0.0, "Sphere position should be (0, 0, 0) (y)");
    cr_assert_eq(sphere.getPosition()._z, 0.0, "Sphere position should be (0, 0, 0) (z)");
}

Test(sphere, get_normal_at)
{
    const raytracer::shape::Sphere sphere(math::Point3D(0, 0, 0), 1.0);
    const math::Point3D point(1, 0, 0);
    const math::Vector3D normal = sphere.getNormalAt(point);
    cr_assert_eq(normal._x, 1.0, "Sphere normal at (1, 0, 0) should be (1, 0, 0) (x)");
    cr_assert_eq(normal._y, 0.0, "Sphere normal at (1, 0, 0) should be (1, 0, 0) (y)");
    cr_assert_eq(normal._z, 0.0, "Sphere normal at (1, 0, 0) should be (1, 0, 0) (z)");
}
