/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D.cpp
*/

#include "Vector3D.hpp"
#include <cmath>

/*
 * public
 */

math::Vector3D::Vector3D() : _x(0), _y(0), _z(0)
{
    /* empty __ctor__ */
}

math::Vector3D::Vector3D(double all) : _x(all), _y(all), _z(all)
{
}

math::Vector3D::Vector3D(const double x, const double y, const double z) : _x(x), _y(y), _z(z)
{
    /* empty __ctor__ */
}

double math::Vector3D::length() const
{
    return std::sqrt(_x * _x + _y * _y + _z * _z);
}

math::Vector3D math::Vector3D::operator+(const Vector3D &other) const
{
    return Vector3D(_x + other._x, _y + other._y, _z + other._z);
}

math::Vector3D &math::Vector3D::operator+=(const Vector3D &other)
{
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
}

math::Vector3D math::Vector3D::operator-() const
{
    return Vector3D(-_x, -_y, -_z);
}

math::Vector3D math::Vector3D::operator-(const Vector3D &other) const
{
    return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}

math::Vector3D &math::Vector3D::operator-=(const Vector3D &other)
{
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
}

math::Vector3D math::Vector3D::operator*(const Vector3D &other) const
{
    return Vector3D(_x * other._x, _y * other._y, _z * other._z);
}

math::Vector3D &math::Vector3D::operator*=(const Vector3D &other)
{
    _x *= other._x;
    _y *= other._y;
    _z *= other._z;
    return *this;
}

math::Vector3D math::Vector3D::operator*(const double scalar) const
{
    return Vector3D(_x * scalar, _y * scalar, _z * scalar);
}

math::Vector3D &math::Vector3D::operator*=(const double scalar)
{
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
    return *this;
}

math::Vector3D math::Vector3D::operator/(const Vector3D &other) const
{
    return Vector3D(_x / other._x, _y / other._y, _z / other._z);
}

math::Vector3D &math::Vector3D::operator/=(const Vector3D &other)
{
    _x /= other._x;
    _y /= other._y;
    _z /= other._z;
    return *this;
}

math::Vector3D math::Vector3D::operator/(const double scalar) const
{
    return Vector3D(_x / scalar, _y / scalar, _z / scalar);
}

math::Vector3D &math::Vector3D::operator/=(const double scalar)
{
    _x /= scalar;
    _y /= scalar;
    _z /= scalar;
    return *this;
}

double math::Vector3D::dot(const Vector3D &other) const
{
    return _x * other._x + _y * other._y + _z * other._z;
}

math::Vector3D math::Vector3D::normalize() const
{
    const double len = length();

    return len != 0.0 ? *this / len : Vector3D(0.0, 0.0, 0.0);
}

math::Vector3D math::Vector3D::cross(const Vector3D &other) const
{
    return Vector3D(_y * other._z - _z * other._y, _z * other._x - _x * other._z, _x * other._y - _y * other._x);
}

bool math::Vector3D::operator==(const Vector3D &other) const
{
    return _x == other._x && _y == other._y && _z == other._z;
}

bool math::Vector3D::operator!=(const Vector3D &other) const
{
    return _x != other._x || _y != other._y || _z != other._z;
}

std::ostream &math::operator<<(std::ostream &os, const math::Vector3D &self)
{
    return (os << "{" << self._x << ", " << self._y << ", " << self._z << "}");
}

void math::Vector3D::realign(double oldMaximum, int maximum)
{
    _x = std::min(oldMaximum, std::max(0.0, _x));
    _y = std::min(oldMaximum, std::max(0.0, _y));
    _z = std::min(oldMaximum, std::max(0.0, _z));
    _x = std::min(maximum, std::max(0, int(_x * maximum)));
    _y = std::min(maximum, std::max(0, int(_y * maximum)));
    _z = std::min(maximum, std::max(0, int(_z * maximum)));
}
