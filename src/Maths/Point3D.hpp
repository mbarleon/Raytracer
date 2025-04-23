#pragma once

#include "Vector3D.hpp"

namespace Math {

class Point3D
{
    public:
        double _x;
        double _y;
        double _z;

        Point3D() = default;
        Point3D(double x, double y, double z);

        Point3D operator+(const Vector3D &vector) const;
        Point3D &operator+=(const Vector3D &vector);
        Point3D operator-(const Vector3D &vector) const;
        Point3D &operator-=(const Vector3D &vector);
        Vector3D operator-(const Point3D &other) const;
};
}// namespace Math
