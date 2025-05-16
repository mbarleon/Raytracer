/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Macro.hpp
*/

#pragma once

#define SUCCESS 0
#define ERROR 84

#if defined(_MSC_VER)
    #define RESTRICT __restrict
#elif defined(__GNUC__)
    #define RESTRICT __restrict__
#else
    #define RESTRICT
#endif

#if defined(UNIT_TESTS)
    #define unit_static
#else
    #define unit_static static
#endif

/**
* @brief DEFAULT MACROS
* @details default general macros to avoid magic numbers everywhere
*/

#define DEFAULT_IDENTATION 0
#define DEFAULT_FIELD_OF_VIEW 70

/**
* @brief Math Pi / 180.0
*/
#define DEGREE_TO_RADIANT 0.017453292519943295

#define MAX_COLOR 255.0
#define MIN_COLOR 0.0

static constexpr double EPSILON = 1e-8;
