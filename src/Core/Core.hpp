/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../Elements/Scene/Shapes/IShape.hpp"
#include "../Elements/Scene/Material/Material.hpp"
#include "../Elements/Render/Render.hpp"
#include "Macro.hpp"
#include <unordered_map>
#include <memory>
#include <vector>

namespace raytracer {
class Core final
{
    public:
        inline explicit Core() = default;
        ~Core() = default;

        void run(const char *RESTRICT filename);

    private:
        std::vector<std::shared_ptr<shape::IShape>> _shapes;
        std::unordered_map<std::string, std::shared_ptr<Material>> _materials;
        std::unique_ptr<Render> _render;
        std::unique_ptr<Camera> _camera;
};
}// namespace raytracer
