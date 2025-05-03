/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShape
*/

#include "AShape.hpp"

void raytracer::shape::AShape::setMaterial(const std::shared_ptr<Material> &material)
{
    _material = material;
}

std::shared_ptr<raytracer::Material> raytracer::shape::AShape::getMaterial() const
{
    return _material;
}
