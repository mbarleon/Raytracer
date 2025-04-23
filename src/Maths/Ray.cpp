/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray.hpp
*/

#include "Ray.hpp"

/**
* @brief
* @details
* @return
*/
math::Point3D math::Ray::at(const double t) const noexcept
{
    return _origin + t * _dir;
}
