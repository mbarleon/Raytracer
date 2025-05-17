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

void raytracer::shape::AShape::setShininess(const double shininess)
{
    _shininess = shininess;
}

void raytracer::shape::AShape::setTexture(const std::shared_ptr<texture::ITexture> &texture)
{
    _texture = texture;
}

raytracer::material::Material raytracer::shape::AShape::getMaterial() const
{
    return _material;
}

double raytracer::shape::AShape::getShininess() const
{
    return _shininess;
}
