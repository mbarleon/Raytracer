/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Color
*/

#include "Color.hpp"
#include <cmath>

std::ostream &raytracer::operator<<(std::ostream &os, const RGBColor &self)
{
    os << self.r << ' ' << self.g << ' ' << self.b;
    return os;
}

raytracer::RGBColor::RGBColor() : r(0), g(0), b(0)
{
}

raytracer::RGBColor::RGBColor(double all) : r(all), g(all), b(all)
{
}

raytracer::RGBColor::RGBColor(double red, double green, double blue) : r(red), g(green), b(blue)
{
}

raytracer::RGBColor raytracer::RGBColor::operator+(const RGBColor &other) const
{
    return RGBColor(r + other.r, g + other.g, b + other.b);
}

raytracer::RGBColor raytracer::RGBColor::operator-(const RGBColor other) const
{
    return RGBColor(r - other.r, g - other.g, b - other.b);
}

raytracer::RGBColor raytracer::RGBColor::operator*(const RGBColor &other) const
{
    return RGBColor(r * other.r, g * other.g, b * other.b);
}

raytracer::RGBColor raytracer::RGBColor::operator*(double s) const
{
    return RGBColor(r * s, g * s, b * s);
}

raytracer::RGBColor raytracer::RGBColor::operator/(double s) const
{
    return RGBColor(r / s, g / s, b / s);
}

double raytracer::RGBColor::length() const
{
    return std::sqrt(r * r + g * g + b * b);
}

void raytracer::RGBColor::realign(double oldMaximum, int maximum)
{
    r = std::min(oldMaximum, std::max(0.0, r));
    g = std::min(oldMaximum, std::max(0.0, g));
    b = std::min(oldMaximum, std::max(0.0, b));
    r = std::min(maximum, std::max(0, int(r * maximum)));
    g = std::min(maximum, std::max(0, int(g * maximum)));
    b = std::min(maximum, std::max(0, int(b * maximum)));
}
