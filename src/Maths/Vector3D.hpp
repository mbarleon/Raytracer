#pragma once

#include <cmath>

namespace Math {
class Vector3D
{
    public:
        double _x;
        double _y;
        double _z;

        Vector3D() = default;
        Vector3D(double x, double y, double z);

        double length() const;

        Vector3D operator+(const Vector3D &other) const;
        Vector3D &operator+=(const Vector3D &other);

        Vector3D operator-(const Vector3D &other) const;
        Vector3D &operator-=(const Vector3D &other);

        Vector3D operator*(const Vector3D &other) const;
        Vector3D &operator*=(const Vector3D &other);

        Vector3D operator/(const Vector3D &other) const;
        Vector3D &operator/=(const Vector3D &other);

        Vector3D operator*(double scalar) const;
        Vector3D &operator*=(double scalar);

        Vector3D operator/(double scalar) const;
        Vector3D &operator/=(double scalar);

        double dot(const Vector3D &other) const;
};

inline Vector3D operator*(double scalar, const Vector3D &vector)
{
    return vector * scalar;
}
}// namespace Math
