/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../Elements/Scene/Materials/Material.hpp"
#include "../Elements/Render/RenderConfig.hpp"
#include "Macro.hpp"
#include <unordered_map>
#include <memory>

namespace raytracer {
class Core final
{
    public:
        inline explicit Core() = default;
        ~Core() = default;

        void run(const char *RESTRICT filename);

    private:
        std::vector<std::shared_ptr<shape::IShape>> _shapes;
        std::vector<std::shared_ptr<light::ILight>> _lights;
        std::unique_ptr<RenderConfig> _render;
        std::unique_ptr<Camera> _camera;
};
}// namespace raytracer
