/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ImageTexture
*/

#include "ImageTexture.hpp"

raytracer::texture::ImageTexture::ImageTexture(const std::string __attribute__((unused)) &filename)
{
}

raytracer::texture::ImageTexture::~ImageTexture() = default;

math::RGBColor raytracer::texture::ImageTexture::value(const math::Point3D __attribute__((unused)) &p,
    const double __attribute__((unused)) u, const double __attribute__((unused)) v) const noexcept
{
    return math::RGBColor(1);
}
