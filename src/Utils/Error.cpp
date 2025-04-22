/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Error.cpp
*/

#include "Error.hpp"

/*
 * public
 */

/**
 * @brief Error::where
 * @details where the Error was raised
 * @return const char *
 */
const char* raytracer::exception::Error::where(void) const noexcept
{
    return _where.c_str();
}

/**
 * @brief Error::what
 * @details why the Error was raised
 * @return const char *
 */
const char* raytracer::exception::Error::what(void) const noexcept
{
    return _what.c_str();
}
