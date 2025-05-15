/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector2u.hpp
*/

#pragma once

#include <ostream>
#include <type_traits>

namespace math {

using uint = unsigned int;

// clang-format off
/**
 * @brief Vector2u
 * @details Define a Vector of two unsigned int {x, y}
 */
typedef struct Vector2u {
    uint _x;
    uint _y;

    /**
     * @brief __ctor__
     * @details construct a vector2u with <Any> x and <Any> y
     */
    template<typename T, typename U, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<U>>>
    Vector2u(T x, U y) : _x(static_cast<uint>(x)), _y(static_cast<uint>(y))
    {
        /* __ctor__ */
    }

} Vector2u;
// clang-format on

/**
 * @brief operator `<<` (iostream | ostream)
 * @details this operator is declared outside Vector2u to avoid `friend` keyword.
 * it allows you to std::cout << "vector: " << my_vector2u << std::endl;
 * the vector is shown as: `{x, y}`
 * @return the result (vector)
 */
std::ostream &operator<<(std::ostream &os, const Vector2u &self);

}// namespace math
