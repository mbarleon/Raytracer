/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIScenePreview.hpp
*/

#pragma once

#include "IWidget.hpp"
#define RT_SFML_USE_INLINES
#include "SFMLMacros.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace raytracer::ui {

/**
* @class UIScenePreview
* @brief Class for the scene preview in the UI.
* @pattern Composite desing pattern (concrete final class)
*/
class UIScenePreview final : public IWidget
{
    public:
        explicit UIScenePreview(const PixelBuffer &buffer);
        ~UIScenePreview() override = default;

        void render(sf::RenderWindow &window) noexcept override;
        void update(const float dt) noexcept override;
        void onEvent(const std::optional<sf::Event> &event, const sf::RenderWindow &window) noexcept override;
        void setImage(const PixelBuffer &buffer) noexcept;

    private:
        PixelBuffer _buffer;
        sf::Texture _texture;
        sf::Sprite _sprite;
};

}// namespace raytracer::ui
