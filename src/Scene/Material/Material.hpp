/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#pragma once

#include "../../../include/Types.hpp"

namespace raytracer {
    typedef struct material {
        types::RGB_color color;
        double reflectivity;
        double transparency;
        double refractiveIndex;
    } Material;
};
