/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CoreRender.cpp
*/

#include "CoreRender.hpp"

// clang-format off

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

// clang-format on
