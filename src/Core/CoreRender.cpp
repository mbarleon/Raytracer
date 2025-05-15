/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.cpp
*/

#include "CoreRender.hpp"

// clang-format off

/**
* @brief converts a PixelBuffer to a PPM file
* @param PixelBuffer the PixelBuffer to convert.
* @param filename the name of the output file. (default is "output.ppm")
* @return void
*/
void raytracer::core::Render::toPPM(const sf::Image &image, const char *filename)
{
    std::ofstream ppm(filename);

    if (!ppm) {
        throw exception::Error("Render", "Unable to open output file.");
    }

    const uint width = image.getSize().x;
    const uint height = image.getSize().y;

    ppm << "P3\n" << width << " " << height << "\n255\n";
    for (uint y = 0; y < height; ++y) {
        for (uint x = 0; x < width; ++x) {
            sf::Color color = image.getPixel(x, y);
            ppm << static_cast<int>(color.r) << ' '
                << static_cast<int>(color.g) << ' '
                << static_cast<int>(color.b) << '\n';
        }
    }
}

/**
* @brief toPreview
* @details generates a preview image of the scene using the provided shapes and camera.
* @param shapes the list of shapes in the scene.
* @param camera the camera used to render the scene.
*/
const raytracer::PixelBuffer raytracer::core::Render::toPreview(const IShapesList &shapes, const Camera &camera)
{
    const math::Vector2u resolution = camera.getResolution();
    const uint width = resolution._x;
    const uint height = resolution._y;

    std::vector<std::vector<math::RGBColor>> grid2d(height, std::vector<math::RGBColor>(width));

    _forEach([&](math::RGBColor &pixel, const math::Vector2u &pos) {
        const double u = static_cast<double>(pos._x) / static_cast<double>(width);
        const double v = static_cast<double>(pos._y) / static_cast<double>(height);

        math::Ray ray;
        camera.generateRay(u, v, ray);

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
#include "Logger.hpp"

/**
* @brief Render::toImageFmt
* @details render a PixelBuffer to <filename>. If no filename is precised, defaulting to `output.png`
* @return void
*/
void raytracer::core::Render::toImageFmt(const PixelBuffer &buffer, const char *filename)
{
    if (buffer.getSize().x == 0 || buffer.getSize().y == 0) {
        logger::debug("Render::toImageFmt: Cannot save empty image");
        return;
    }
    if (!buffer.saveToFile(filename ? filename : "output.png")) {
        logger::debug("Render::toImageFmt: Failed to save image to ", filename);
    }
}

// clang-format on
