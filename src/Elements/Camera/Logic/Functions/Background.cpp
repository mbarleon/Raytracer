/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Background
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::getBackgroundColor(const math::Vector3D &v)
{
    const math::Vector3D dir = v / v.length();
    const auto t = 0.5 * (dir._y + 1.0);

    return (1.0 - t) * math::RGBColor(1) + t * math::RGBColor(0.3, 0.5, 1.0);
}
