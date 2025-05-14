/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector2u.cpp
*/

#include "Vector2u.hpp"

std::ostream &math::operator<<(std::ostream &os, const math::Vector2u &self)
{
    return (os << "{" << self._x << ", " << self._y << "}");
}
