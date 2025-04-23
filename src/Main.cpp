/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main.cpp
*/

#include "Maths/Point3D.hpp"
#include "Maths/Vector3D.hpp"
#include "RayTracer/Camera.hpp"
#include "RayTracer/Sphere.hpp"
#include <iostream>

void write_color(const Math::Vector3D &color)
{
    const int r = static_cast<int>(255.999 * color._x);
    const int g = static_cast<int>(255.999 * color._y);
    const int b = static_cast<int>(255.999 * color._z);

    std::cout << r << ' ' << g << ' ' << b << '\n';
}

int main(void)
{
    constexpr int image_width = 400;
    constexpr int image_height = 400;

    RayTracer::Camera cam;
    RayTracer::Sphere s(Math::Point3D(0, 0, -1), 0.5);

    std::cout << "P3" << std::endl << image_width << ' ' << image_height << std::endl << "255" << std::endl;

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {

            const double u = static_cast<double>(i) / (image_width - 1);
            const double v = static_cast<double>(j) / (image_height - 1);

            RayTracer::Ray r = cam.ray(u, v);

            if (s.hit(r)) {
                write_color(Math::Vector3D(1.0, 0.0, 0.0));
            } else {
                write_color(Math::Vector3D(0.0, 0.0, 1.0));
            }
        }
    }
    return 0;
}
