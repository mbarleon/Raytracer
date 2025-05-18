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

void raytracer::shape::AShape::setNormalMap(const std::shared_ptr<texture::ITexture> &map, const double strength)
{
    _normalMap = map;
    _normalStrength = strength;
}

raytracer::material::Material raytracer::shape::AShape::getMaterial() const
{
    return _material;
}

double raytracer::shape::AShape::getShininess() const
{
    return _shininess;
}

math::Vector3D raytracer::shape::AShape::getMappedNormal(const math::Point3D &p) const noexcept
{
    const math::Vector3D N = getNormalAt(p);

    if (_normalMap == nullptr) {
        return N;
    }

    double u;
    double v;
    getUV(p, u, v);

    // if (u == 0 && v == 0) {
    //     return N;
    // }

    const math::Vector3D T = N.orthogonal().normalize();
    const math::Vector3D B = N.cross(T).normalize();

    const math::RGBColor rgb = _normalMap->value(p, u, v);
    math::Vector3D nt(rgb._x * 2.0 - 1.0, rgb._y * 2.0 - 1.0, rgb._z * 2.0 - 1.0);

    nt._x *= _normalStrength;
    nt._y *= _normalStrength;
    return (T * nt._x + B * nt._y + N * nt._z).normalize();
}
