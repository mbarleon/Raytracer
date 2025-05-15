/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** UIContainer.hpp
*/

#pragma once

#include "IWidget.hpp"
#include <memory>
#include <vector>

namespace raytracer::ui {

/**
* @class Container
* @details Container widget that can hold other widgets
* @pattern Composite pattern (abstract)
*/
class Container : public IWidget
{
    public:
        explicit Container() = default;
        ~Container() = default;

        void addWidget(const std::shared_ptr<IWidget> widget) noexcept;
        void removeWidget(std::shared_ptr<IWidget> widget) noexcept;

        void update(const float dt) noexcept override;
        void render(sf::RenderWindow &window) noexcept override;
        void onEvent(const sf::Event &event, const sf::RenderWindow &window) noexcept override;

    protected:
        std::vector<std::shared_ptr<IWidget>> _children;
};
}// namespace raytracer::ui
