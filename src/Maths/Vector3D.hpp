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

        constexpr explicit Vector3D() = default;
        constexpr explicit Vector3D(const double x, const double y, const double z);

        constexpr double length() const;

        constexpr Vector3D operator+(const Vector3D &other) const;
        constexpr Vector3D &operator+=(const Vector3D &other);

        constexpr Vector3D operator-(const Vector3D &other) const;
        constexpr Vector3D &operator-=(const Vector3D &other);

        constexpr Vector3D operator*(const Vector3D &other) const;
        constexpr Vector3D &operator*=(const Vector3D &other);

        constexpr Vector3D operator/(const Vector3D &other) const;
        constexpr Vector3D &operator/=(const Vector3D &other);

        Vector3D operator*(const double scalar) const;
        constexpr Vector3D &operator*=(const double scalar);

        constexpr Vector3D operator/(const double scalar) const;
        constexpr Vector3D &operator/=(const double scalar);

        constexpr double dot(const Vector3D &other) const;
        constexpr Vector3D normalize() const;
        constexpr Vector3D cross(const Vector3D &other) const;
};

/**
 * @brief operator `*` (scalar, vector)
 * @details this operator is inline because you cannot overload it in the class !
 * @return the result (vector)
 */
inline constexpr Vector3D operator*(const double scalar, const Vector3D &vector)
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
