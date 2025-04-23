#include "Point3D.hpp"

Math::Point3D::Point3D(double x, double y, double z) : _x(x), _y(y), _z(z)
{
    /* empty */
}

Math::Point3D Math::Point3D::operator+(const Vector3D &vector) const
{
    return Point3D(_x + vector._x, _y + vector._y, _z + vector._z);
}

Math::Point3D &Math::Point3D::operator+=(const Vector3D &vector)
{
    _x += vector._x;
    _y += vector._y;
    _z += vector._z;
    return *this;
}

Math::Point3D Math::Point3D::operator-(const Vector3D &vector) const
{
    return Point3D(_x - vector._x, _y - vector._y, _z - vector._z);
}

Math::Point3D &Math::Point3D::operator-=(const Vector3D &vector)
{
    _x -= vector._x;
    _y -= vector._y;
    _z -= vector._z;
    return *this;
}

Math::Vector3D Math::Point3D::operator-(const Math::Point3D &other) const
{
    return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}
