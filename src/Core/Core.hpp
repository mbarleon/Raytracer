/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.hpp
*/

#pragma once

#include "../Shapes/IShape.hpp"
#include "Macro.hpp"
#include <memory>
#include <vector>

namespace raytracer {
class Core final
{
    public:
        constexpr explicit Core() = default;
        constexpr ~Core() = default;

        void run(const char *RESTRICT filename);

    private:
        std::vector<std::shared_ptr<shape::IShape>> _shapes;
};
}// namespace raytracer
