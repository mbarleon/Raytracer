/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.hpp
*/

#pragma once

// #include "../Elements/Camera/Camera.hpp"
// #include "../Elements/Scene/Materials/Material.hpp"
// #include "../Elements/Render/Render.hpp"
#include "../Elements/Render/SFML/Backend/IBackend.hpp"
#include "../Elements/Render/SFML/Frontend/IFrontend.hpp"
#include "Macro.hpp"
#include <memory>
#include <unordered_map>

namespace raytracer::core {

#define SFML_MENU "SFMLMenu"

using BackPtr = std::unique_ptr<backend::IBackend>;
using FrontMap = std::unordered_map<std::string, std::unique_ptr<frontend::IFrontend>>;

class Core final
{
    public:
        inline explicit Core() = default;
        ~Core() = default;

        void run(const char *RESTRICT filename);

    private:
        void _render() const noexcept;
        void _render_front() const noexcept;

        BackPtr _backend;
        FrontMap _frontends;
        // std::vector<std::shared_ptr<shape::IShape>> _shapes;
        // std::vector<std::shared_ptr<light::ILight>> _lights;
        // std::unique_ptr<Render> _render;
        // std::unique_ptr<Camera> _camera;
};
}// namespace raytracer::core
