#include "Vector3D.hpp"

Math::Vector3D::Vector3D(double x, double y, double z) : _x(x), _y(y), _z(z)
{
    /* empty */
}

double Math::Vector3D::length() const
{
    return std::sqrt(_x * _x + _y * _y + _z * _z);
}

Math::Vector3D Math::Vector3D::operator+(const Vector3D &other) const
{
    return Vector3D(_x + other._x, _y + other._y, _z + other._z);
}

Math::Vector3D &Math::Vector3D::operator+=(const Vector3D &other)
{
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator-(const Vector3D &other) const
{
    return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}

Math::Vector3D &Math::Vector3D::operator-=(const Vector3D &other)
{
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator*(const Vector3D &other) const
{
    return Vector3D(_x * other._x, _y * other._y, _z * other._z);
}

Math::Vector3D &Math::Vector3D::operator*=(const Vector3D &other)
{
    _x *= other._x;
    _y *= other._y;
    _z *= other._z;
    return *this;
}

Math::Vector3D Math::Vector3D::operator*(double scalar) const
{
    return Vector3D(_x * scalar, _y * scalar, _z * scalar);
}

Math::Vector3D &Math::Vector3D::operator*=(double scalar)
{
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
    return *this;
}

Math::Vector3D Math::Vector3D::operator/(const Vector3D &other) const
{
    return Vector3D(_x / other._x, _y / other._y, _z / other._z);
}

Math::Vector3D &Math::Vector3D::operator/=(const Vector3D &other)
{
    _x /= other._x;
    _y /= other._y;
    _z /= other._z;
    return *this;
}

double Math::Vector3D::dot(const Vector3D &other) const
{
    return _x * other._x + _y * other._y + _z * other._z;
}
