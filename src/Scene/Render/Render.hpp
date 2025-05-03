/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Render
*/

#pragma once

#include "../Material/Color.hpp"
#include "Antialiasing.hpp"
#include "RenderOutput.hpp"

namespace raytracer {
    typedef struct render {
        RGB_color _background;
        Antialiasing _antialiasing;
        RenderOutput _output;
        unsigned int _maxDepth;
    } Render;
};
