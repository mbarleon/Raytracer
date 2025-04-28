/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Glass.hpp
*/

#pragma once

#include "AMaterial.hpp"

namespace raytracer::material {
class Glass final : public AMaterial
{
    public:
        Glass() = default;
        ~Glass() override = default;
};
}// namespace raytracer::material
