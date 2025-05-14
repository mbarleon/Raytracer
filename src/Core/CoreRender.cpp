/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.cpp
*/

#include "CoreRender.hpp"
#include "../Maths/Vector2u.hpp"
#include "Error.hpp"
#include <fstream>
#include <iostream>

// clang-format off

/**
 * @brief applies a function to each pixel in the RaytraceGrid2D.
 * @param func the function to apply.
 * @param grid2 the RaytraceGrid2D to iterate over.
 * @return void
 */
template<typename Func>
void _loop(const Func &&func, const raytracer::RaytraceGrid2D &grid2) noexcept
{
    const size_t width = grid2.front().size();
    const size_t height = grid2.size();

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            func(grid2[y][x], math::Vector2u(static_cast<uint>(x), static_cast<uint>(y)));
        }
    }
}

/**
* @brief converts a RaytraceGrid2D to a PPM file.
* @param grid2 the RaytraceGrid2D to convert.
* @param filename the name of the output file. (default is "output.ppm")
* @return void
*/
void raytracer::core::Render::toPPM(const RaytraceGrid2D &grid2, const char *filename)
{
    std::ofstream ppm(filename);

    if (!ppm) {
        throw exception::Error("Camera", "Unable to open output file.");
    }

    ppm << "P3\n" << grid2.front().size() << " " << grid2.size() << "\n255\n";
    _loop( [&ppm] (const auto &pixel, const math::Vector2u __attribute__((unused)) & pos)
           { 
               math::RGBColor color = pixel.estimate();
               color.realign();
               ppm << static_cast<int>(color._x) << ' ' << static_cast<int>(color._y) << ' ' << static_cast<int>(color._z) << '\n';
           },
    grid2);
}

/**
* @brief converts a RaytraceGrid2D to an sf::Image.
* @param grid2 the RaytraceGrid2D to convert.
* @return the converted sf::Image.
*/
const sf::Image raytracer::core::Render::toImage(const RaytraceGrid2D &grid2)
{
    sf::Image image;

    image.create(static_cast<uint>(grid2.front().size()), static_cast<uint>(grid2.size()));
    _loop( [&image] (const auto &pixel, const math::Vector2u &pos)
           {
               math::RGBColor rgbc = pixel.estimate();
               rgbc.realign();
               const sf::Color color(static_cast<uint8_t>(rgbc._x), static_cast<uint8_t>(rgbc._y), static_cast<uint8_t>(rgbc._z));
               image.setPixel(pos._x, pos._y, color);
           },
    grid2);

    return image;
}

// clang-format on
