/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIScenePreview.hpp
*/

#pragma once

#include "IWidget.hpp"
#include "Macro.hpp"

#include "../Elements/Scene/Lights/ILight.hpp"
#include "../Elements/Scene/Materials/Material.hpp"
#include "../Elements/Scene/Shapes/IShape.hpp"

namespace raytracer::ui {

using IShapesList = std::vector<std::shared_ptr<raytracer::shape::IShape>>;
using ILightsList = std::vector<std::shared_ptr<raytracer::light::ILight>>;

/**
* @class UIScenePreview
* @brief Class for the scene preview in the UI.
* @pattern Composite desing pattern (concrete final class)
*/
class UIScenePreview final : public IWidget
{
    public:
        explicit UIScenePreview(const char *RESTRICT filename);
        ~UIScenePreview() override = default;

        void render(sf::RenderWindow &window) noexcept override;
        void update(const float dt) noexcept override;
        void events(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    private:
        IShapesList _shapes;
        ILightsList _lights;
};
}// namespace raytracer::ui
