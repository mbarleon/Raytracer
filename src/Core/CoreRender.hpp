/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.hpp
*/

#pragma once

#include "../Elements/Camera/Logic/Pathtracer.hpp"
#include <SFML/Graphics/Image.hpp>

namespace raytracer::core {

/**
* @class Render
* @brief Singleton class to render objects to format
*/
class Render
{
    public:
        static void toPPM(const RaytraceGrid2D &grid2, const char *filename = "output.ppm");
        static void toPNG(const RaytraceGrid2D &grid2, const char *filename = "output.png");
        static const sf::Image toImage(const RaytraceGrid2D &grid2);

    private:
        explicit Render() = default;

        constexpr Render(const Render &) = delete;
        constexpr Render(Render &&) = delete;
        constexpr Render &operator=(const Render &) = delete;
        constexpr Render &operator=(Render &&) = delete;
};
}// namespace raytracer::core
