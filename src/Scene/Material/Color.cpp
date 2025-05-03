/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color
*/

#include "Color.hpp"

std::ostream &raytracer::operator<<(std::ostream &os, const RGB_color &self)
{
    os << static_cast<int>(self.r) << ' '
       << static_cast<int>(self.g) << ' '
       << static_cast<int>(self.b);
    return os;
}
