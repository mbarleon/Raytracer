/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.hpp
*/

#pragma once

#include "../Camera/Camera.hpp"
#include "../Shapes/IShape.hpp"
#include "Macro.hpp"
#include <memory>
#include <vector>

namespace raytracer {
class Core final
{
    public:
        constexpr explicit Core() = default;
        ~Core() = default;

        void run(const char *RESTRICT filename);

    private:
        std::vector<std::shared_ptr<shape::IShape>> _shapes;
        std::unique_ptr<Camera> _camera;
};
}// namespace raytracer
