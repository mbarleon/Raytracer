/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AMaterial.hpp
*/

#pragma once

#include "IMaterial.hpp"

namespace raytracer::material {
class AMaterial : public IMaterial
{
    public:
        AMaterial() = default;
        ~AMaterial() override = default;

        /*void setColor(const types::color_u &color);*/
};
}// namespace raytracer::material
