/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Types
*/

#pragma once

#include <iostream>

namespace raytracer {
    typedef struct RGBColor {
        double r;
        double g;
        double b;

        RGBColor();
        RGBColor(double red, double green, double blue);
        RGBColor operator+(const RGBColor &other) const;
        RGBColor operator*(double s) const;
        void realign(int maximum);
    } RGBColor;

    std::ostream &operator<<(std::ostream &os, const RGBColor &self);
};
