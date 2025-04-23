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

constexpr math::Vector3D::Vector3D(const double x, const double y, const double z) : _x(x), _y(y), _z(z)
{
    /* empty */
}

constexpr double math::Vector3D::length() const
{
    return std::sqrt(_x * _x + _y * _y + _z * _z);
}

constexpr math::Vector3D math::Vector3D::operator+(const Vector3D &other) const
{
    return Vector3D(_x + other._x, _y + other._y, _z + other._z);
}

constexpr math::Vector3D &math::Vector3D::operator+=(const Vector3D &other)
{
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
}

constexpr math::Vector3D math::Vector3D::operator-(const Vector3D &other) const
{
    return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}

constexpr math::Vector3D &math::Vector3D::operator-=(const Vector3D &other)
{
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
}

constexpr math::Vector3D math::Vector3D::operator*(const Vector3D &other) const
{
    return Vector3D(_x * other._x, _y * other._y, _z * other._z);
}

constexpr math::Vector3D &math::Vector3D::operator*=(const Vector3D &other)
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

constexpr math::Vector3D &math::Vector3D::operator*=(const double scalar)
{
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
    return *this;
}

constexpr math::Vector3D math::Vector3D::operator/(const Vector3D &other) const
{
    return Vector3D(_x / other._x, _y / other._y, _z / other._z);
}

constexpr math::Vector3D &math::Vector3D::operator/=(const Vector3D &other)
{
    _x /= other._x;
    _y /= other._y;
    _z /= other._z;
    return *this;
}

constexpr math::Vector3D math::Vector3D::operator/(const double scalar) const
{
    return Vector3D(_x / scalar, _y / scalar, _z / scalar);
}

constexpr math::Vector3D &math::Vector3D::operator/=(const double scalar)
{
    _x /= scalar;
    _y /= scalar;
    _z /= scalar;
    return *this;
}

constexpr double math::Vector3D::dot(const Vector3D &other) const
{
    return _x * other._x + _y * other._y + _z * other._z;
}

constexpr math::Vector3D math::Vector3D::normalize() const
{
    const double len = length();

    return len != 0.0 ? *this / len : Vector3D(0.0, 0.0, 0.0);
}

constexpr math::Vector3D math::Vector3D::cross(const Vector3D &other) const
{
    return Vector3D(_y * other._z - _z * other._y, _z * other._x - _x * other._z, _x * other._y - _y * other._x);
}
