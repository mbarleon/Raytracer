/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#pragma once

#include "Color.hpp"

namespace raytracer {
    typedef struct material {
        RGB_color color;
        double reflectivity;
        double transparency;
        double refractiveIndex;
    } Material;
};
