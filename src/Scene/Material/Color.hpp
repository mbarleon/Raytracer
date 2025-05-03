/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Types
*/

#pragma once

#include <iostream>

namespace raytracer {
    typedef struct rgb_color_s {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    } RGB_color;

    std::ostream &operator<<(std::ostream &os, const RGB_color &self);
};
