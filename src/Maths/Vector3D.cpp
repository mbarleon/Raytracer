/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D.cpp
*/

#include "Vector3D.hpp"
#include "../../include/Macro.hpp"
#include <cmath>

/*
 * public
 */

math::Vector3D::Vector3D() : _x(0), _y(0), _z(0)
{
    /* empty __ctor__ */
}

math::Vector3D::Vector3D(const double all) : _x(all), _y(all), _z(all)
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

double math::Vector3D::lengthSquared() const
{
    return _x * _x + _y * _y + _z * _z;
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

math::Vector3D math::Vector3D::orthonormal() const
{
    if (std::fabs(_x) > std::fabs(_z)) {
        return math::Vector3D(-_y, _x, 0.0);
    }
    return math::Vector3D(0.0, -_z, _y);
}

math::Vector3D math::Vector3D::cross(const Vector3D &other) const
{
    return Vector3D(_y * other._z - _z * other._y, _z * other._x - _x * other._z, _x * other._y - _y * other._x);
}

double math::Vector3D::maxComponent() const
{
    if (_x > _y && _x > _z)
        return _x;
    if (_y > _x && _y > _z)
        return _y;
    return _z;
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

bool math::Vector3D::nearZero() const
{
    return (std::fabs(_x) < EPSILON) && (std::fabs(_y) < EPSILON) && (std::fabs(_z) < EPSILON);
}

math::Vector3D math::Vector3D::applyRotation(const math::Vector3D &dir, const math::Vector3D &rot)
{
    const double cosX = std::cos(rot._x), sinX = std::sin(rot._x);
    const double cosY = std::cos(rot._y), sinY = std::sin(rot._y);
    const double cosZ = std::cos(rot._z), sinZ = std::sin(rot._z);

    const double m00 = cosY * cosZ;
    const double m01 = cosZ * sinX * sinY - sinZ * cosX;
    const double m02 = cosZ * cosX * sinY + sinZ * sinX;
    const double m10 = cosY * sinZ;
    const double m11 = sinZ * sinX * sinY + cosZ * cosX;
    const double m12 = sinZ * cosX * sinY - cosZ * sinX;
    const double m20 = -sinY;
    const double m21 = cosY * sinX;
    const double m22 = cosY * cosX;

    return math::Vector3D(m00 * dir._x + m01 * dir._y + m02 * dir._z, m10 * dir._x + m11 * dir._y + m12 * dir._z,
        m20 * dir._x + m21 * dir._y + m22 * dir._z);
}

void math::Vector3D::realign() noexcept
{
    _x = sqrt(_x);
    _y = sqrt(_y);
    _z = sqrt(_z);
    if (_x > 0.999)
        _x = 0.999;
    if (_y > 0.999)
        _y = 0.999;
    if (_z > 0.999)
        _z = 0.999;
    _x *= 256;
    _y *= 256;
    _z *= 256;
}
