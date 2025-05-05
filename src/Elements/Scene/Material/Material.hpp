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
        double reflectivity;
        double transparency;
        double refractiveIndex;
        double emissiveIntensity;
        double shininess;
    } Material;
};
