/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../Elements/Camera/Logic/Pathtracer.hpp"
#include "Error.hpp"
#include "NonCopyable.hpp"
#include "SFMLMacros.hpp"
#include <SFML/Graphics/Image.hpp>
#include <fstream>

namespace raytracer::core {

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
static void _forEach(const Func &&func, Grid &grid, const math::Vector2u size = RT_DEFAULT_SIZE) noexcept
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

static inline double getPixelColor(const double c, const double gamma)
{
    const double a = 2.51, b = 0.03, c0 = 2.43, d = 0.59, e = 0.14;
    double mapped = (c * (a * c + b)) / (c * (c0 * c + d) + e);
    mapped = std::clamp(mapped, 0.0, 1.0);

    const double g = std::pow(mapped, 1.0 / gamma);
    return static_cast<int>(g * 255.0 + 0.5);
}

static inline void realignColor(math::RGBColor &color, const double gamma)
{
    color._x = getPixelColor(color._x, gamma);
    color._y = getPixelColor(color._y, gamma);
    color._z = getPixelColor(color._z, gamma);
};

/**
* @class Render
* @pattern Singleton class to render objects to format
*/
class Render final : public NonCopyable
{
    public:

        /**
        * @brief converts a PixelBuffer to a PPM file
        * @param PixelBuffer the PixelBuffer to convert.
        * @param filename the name of the output file. (default is "output.ppm")
        * @return void
        */
        static void toPPM(const PixelBuffer &buffer, const char *filename = "output.ppm");

        /**
        * @brief converts a RaytraceGrid2D to a PPM file.
        * @param grid2 the RaytraceGrid2D to convert.
        * @param filename the name of the output file. (default is "output.ppm")
        * @return void
        */
        template<typename Grid>
        static void toPPM(const Grid &grid, const char *filename = "output.ppm")
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
                    ppm << static_cast<int>(color._x) << ' ' << static_cast<int>(color._y) << ' ' << static_cast<int>(color._z) << '\n';
                }, const_cast<Grid &>(grid), {width, height});
        }

        static void toImageFmt(const PixelBuffer &buffer, const char *filename = "output.png");

        /**
        * @brief converts a RaytraceGrid2D to an PixelBuffer.
        * @param grid2 the RaytraceGrid2D to convert.
        * @return the converted PixelBuffer.
        */
        template<typename Grid>
        static const PixelBuffer toImage(const Grid &grid, const double gamma)
        {
            const auto height = grid.size();
            const auto width = height > 0 ? grid.front().size() : 0;

            PixelBuffer buffer;
            buffer.create(static_cast<uint>(width), static_cast<uint>(height));

            _forEach([&buffer, &gamma](const auto &pixel, const math::Vector2u &pos)
                {
                    math::RGBColor color;

                    using PixelType = std::decay_t<decltype(pixel)>;
                    if constexpr (has_estimate<PixelType>::value) {
                        color = pixel.estimate();
                    } else {
                        color = pixel;
                    }
                    realignColor(color, gamma);
                    buffer.setPixel(pos._x, pos._y, sf::Color( static_cast<u8>(color._x), static_cast<u8>(color._y), static_cast<u8>(color._z)));
                }, const_cast<Grid &>(grid), {width, height});

            return buffer;
        }

        /**
        * @brief toPreview
        * @details generates a preview image of the scene using the provided shapes and camera.
        * @param shapes the list of shapes in the scene.
        * @param camera the camera used to render the scene.
        */
        static const PixelBuffer toPreview(const IShapesList &shapes, const Camera &camera, const double gamma);

};

// clang-format on

}// namespace raytracer::core
