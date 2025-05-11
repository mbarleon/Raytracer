/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Lighting
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::computeDirectLight(const math::Intersect &intersect,
    const math::Vector3D &V, const material::Material &mat, const IShapesList &shapes,
    const ILightsList &lights, const Render &render)
{
    (void)intersect;
    (void)lights;
    (void)V;
    (void)mat;
    (void)shapes;
    (void)lights;
    (void)render;
    return math::RGBColor(0.5);
}
