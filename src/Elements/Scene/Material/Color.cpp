/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color
*/

#include "Color.hpp"

std::ostream &raytracer::operator<<(std::ostream &os, const RGBColor &self)
{
    os << self.r << ' ' << self.g << ' ' << self.b;
    return os;
}

raytracer::RGBColor::RGBColor(double red, double green, double blue) :
    r(red), g(green), b(blue)
{
}

raytracer::RGBColor raytracer::RGBColor::operator+(const RGBColor &other) const
{
    return RGBColor(r + other.r, g + other.g, b + other.b);
}

raytracer::RGBColor raytracer::RGBColor::operator*(double s) const
{
    return RGBColor(r * s, g * s, b * s);
}
