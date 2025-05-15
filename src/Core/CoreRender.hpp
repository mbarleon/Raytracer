/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../Elements/Camera/Logic/Pathtracer.hpp"
#include "NonCopyable.hpp"
#include <SFML/Graphics/Image.hpp>

namespace raytracer::core {

/**
* @class Render
* @pattern Singleton class to render objects to format
*/
class Render final : public NonCopyable
{
    public:
        template<typename Grid>
        static void toPPM(const Grid &grid, const char *filename = "output.ppm");

        template<typename Grid>
        static void toPNG(const Grid &grid, const char *filename = "output.png");

        template<typename Grid>
        static const sf::Image toImage(const Grid &grid);

        static const sf::Image toPreview(const IShapesList &shapes, const Camera &camera);
};
}// namespace raytracer::core
