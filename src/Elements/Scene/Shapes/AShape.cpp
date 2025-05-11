/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShape
*/

#include "AShape.hpp"

void raytracer::shape::AShape::setMaterial(const material::Material &material)
{
    _material = material;
}

void raytracer::shape::AShape::setColor(math::RGBColor color)
{
    _color = color;
}

raytracer::material::Material raytracer::shape::AShape::getMaterial() const
{
    return _material;
}

math::RGBColor raytracer::shape::AShape::getColor() const
{
    return _color;
}
