/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../Elements/Camera/Logic/Pathtracer.hpp"
#include <SFML/Graphics/Image.hpp>

namespace raytracer::core {

#define DEFAULT_SIZE {1920, 1080}

/**
* @class Render
* @pattern Singleton class to render objects to format
*/
class Render
{
    public:
        template<typename Grid>
        static void toPPM(const Grid &grid, const char *filename = "output.ppm");

        template<typename Grid>
        static void toPNG(const Grid &grid, const char *filename = "output.png");

        template<typename Grid>
        static const sf::Image toImage(const Grid &grid);

        static const sf::Image toPreview(const IShapesList &shapes, const Camera &camera);

    private:
        explicit Render() = default;

        constexpr Render(const Render &) = delete;
        constexpr Render(Render &&) = delete;
        constexpr Render &operator=(const Render &) = delete;
        constexpr Render &operator=(Render &&) = delete;
};
}// namespace raytracer::core
