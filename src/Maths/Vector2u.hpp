/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector2u.hpp
*/

#pragma once

#include <ostream>

namespace math {

using uint = unsigned int;

// clang-format off
/**
 * @brief Vector2u
 * @details Define a Vector of two unsigned int {x, y}
 */
typedef struct Vector2u {
    uint x;
    uint y;
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
