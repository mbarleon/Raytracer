/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include <string>

namespace raytracer {
    typedef struct antialiasing {
        std::string type;
        unsigned int samples;
    } Antialiasing;
};
