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

template<typename T, typename = void>
struct has_estimate : std::false_type
{
    /* */
};

template<typename T>
struct has_estimate<T, std::void_t<decltype(std::declval<T>().estimate())>> : std::true_type
{
    /* */
};

/**
 * @brief applies a function to each pixel in the grid
 * @param func the function to apply.
 * @param grid the element to iterate over
 * @return void
 */
template<typename Func, typename Grid>
static void _forEach(const Func &&func, Grid &grid, const math::Vector2u size = DEFAULT_SIZE) noexcept
{
    if (size._y == 0) {
        return;
    }

    for (size_t y = 0; y < size._y; ++y) {

        for (size_t x = 0; x < size._x; ++x) {
            func(grid[y][x], math::Vector2u(static_cast<uint>(x), static_cast<uint>(y)));
        }

    }

}

/**
* @brief converts a RaytraceGrid2D to a PPM file.
* @param grid2 the RaytraceGrid2D to convert.
* @param filename the name of the output file. (default is "output.ppm")
* @return void
*/
template<typename Grid>
void raytracer::core::Render::toPPM(const Grid &grid, const char *filename)
{
    std::ofstream ppm(filename);

    if (!ppm) {
        throw exception::Error("Camera", "Unable to open output file.");
    }

    const auto height = grid.size();
    const auto width = height > 0 ? grid.front().size() : 0;

    ppm << "P3\n" << width << " " << height << "\n255\n";
    _forEach( [&ppm] (const auto &pixel, const math::Vector2u &)
        {
            math::RGBColor color = pixel.estimate();
            color.realign();
            ppm << static_cast<int>(color._x) << ' ' << static_cast<int>(color._y) << ' ' << static_cast<int>(color._z) << '\n';
        }, const_cast<Grid &>(grid), {width, height});
}

/**
* @brief converts a RaytraceGrid2D to an sf::Image.
* @param grid2 the RaytraceGrid2D to convert.
* @return the converted sf::Image.
*/
template<typename Grid>
const sf::Image raytracer::core::Render::toImage(const Grid &grid)
{
    const auto height = grid.size();
    const auto width = height > 0 ? grid.front().size() : 0;

    sf::Image image;
    image.create(static_cast<uint>(width), static_cast<uint>(height));

    _forEach([&image](const auto &pixel, const math::Vector2u &pos)
        {
            math::RGBColor rgbc;

            using PixelType = std::decay_t<decltype(pixel)>;
            if constexpr (has_estimate<PixelType>::value) {
                rgbc = pixel.estimate();
            } else {
                rgbc = pixel;
            }
            rgbc.realign();

            image.setPixel(pos._x, pos._y, {
                static_cast<sf::Uint8>(std::clamp(rgbc._x, 0.0, 1.0) * 255.0),
                static_cast<sf::Uint8>(std::clamp(rgbc._y, 0.0, 1.0) * 255.0),
                static_cast<sf::Uint8>(std::clamp(rgbc._z, 0.0, 1.0) * 255.0)
            });
        }, const_cast<Grid &>(grid), {width, height});

    return image;
}

const sf::Image raytracer::core::Render::toPreview(const IShapesList &shapes, const Camera &camera)
{
    const auto resolution = camera.getResolution();
    const uint width = resolution._x;
    const uint height = resolution._y;

    sf::Image image;
    image.create(width, height, sf::Color::Black);

    std::vector<std::vector<math::RGBColor>> grid2d(height, std::vector<math::RGBColor>(width));

    const double fov = 60.0;
    const double aspect = static_cast<double>(width) / height;
    const double scale = std::tan(fov * 0.5 * M_PI / 180.0);
    const math::Point3D cameraPos(0, 0, -5);

    _forEach([&](math::RGBColor &pixel, const math::Vector2u &pos) {
        const double px = (2.0 * (pos._x + 0.5) / width - 1.0) * aspect * scale;
        const double py = (1.0 - 2.0 * (pos._y + 0.5) / height) * scale;
        math::Vector3D dir(px, py, 1.0);
        dir = dir.normalize();

        math::Ray ray(cameraPos, dir);

        for (const auto &shape : shapes) {
            math::Point3D hitPoint;
            if (shape->intersect(ray, hitPoint, true)) {
                pixel = shape->getColor();
                break;
            }
        }
    }, grid2d, resolution);

    return toImage(grid2d);
}

// clang-format on
