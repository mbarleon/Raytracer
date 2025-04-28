/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D.hpp
*/

#pragma once

namespace math {
class Vector3D
{
    public:
        double _x;
        double _y;
        double _z;

        explicit Vector3D();
        explicit Vector3D(double x, double y, double z);

        [[nodiscard]] double length() const;

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

        [[nodiscard]] double dot(const Vector3D &other) const;
        [[nodiscard]] Vector3D normalize() const;
        [[nodiscard]] Vector3D cross(const Vector3D &other) const;

        bool operator==(const Vector3D &other) const;
        bool operator!=(const Vector3D &other) const;
};

/**
 * @brief operator `*` (scalar, vector)
 * @details this operator is inline because you cannot overload it in the class !
 * @return the result (vector)
 */
inline Vector3D operator*(const double scalar, const Vector3D &vector)
{
    return vector * scalar;
}

/**
* @brief Point3D expand to Vector3D
* @details no need to redefine a look-a-like class,
* we better have to use a `using` macro to create another type.
*/
using Point3D = Vector3D;
}// namespace math
