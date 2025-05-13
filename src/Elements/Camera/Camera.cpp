/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"
#include "Logger.hpp"
#include <atomic>
#include <fstream>
#include <mutex>
#include <random>
#include <sys/stat.h>
#include <thread>
#include <algorithm>
#include "../../Maths/Intersect.hpp"
#include "../Render/ReSTIR/Tank.hpp"
#include "Logic/Pathtracer.hpp"

// clang-format off

raytracer::Camera::Camera(const math::Vector2u &resolution, const math::Point3D &position,
    const math::Vector3D &rotation, const unsigned int fov) : _resolution(resolution),
    _position(position), _rotation(rotation), _fov(fov)
{
    logger::debug("Camera was built: resolution ", resolution, " position ", position,
        " rotation ", rotation, " fov: ", fov, ".");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

int getPixelColor(double c, double gamma)
{
    double mapped = c / (1.0 + c);
    mapped = std::clamp(mapped, 0.0, 1.0);

    const double g = std::pow(mapped, 1.0 / gamma);
    return static_cast<int>(g * 255.0 + 0.5);
};

void raytracer::Camera::render(const IShapesList &shapes, const ILightsList &lights,
    const Render &render) const
{
    const unsigned int nproc = std::thread::hardware_concurrency();

    if (nproc == 0) {
        throw exception::Error("raytracer::Camera::render()", "No threads available");
    }

    std::vector<std::thread> threads;
    std::mutex progressBarMutex;

    threads.reserve(nproc);

    std::atomic<unsigned> linesDone(0);
    std::vector<std::vector<Tank>> restirGrid(_resolution.y, std::vector<Tank>(_resolution.x));

    const auto sparseWorker = [&](unsigned threadId) {
        std::mt19937 rng = material::getRng(threadId, _resolution.x, _resolution.y);
        math::Ray cameraRay;

        // jitter
        std::uniform_real_distribution<double> jitterX(-0.5, 0.5);
        std::uniform_real_distribution<double> jitterY(-0.5, 0.5);

        for (unsigned y = threadId; y < _resolution.y; y += nproc) {
            const double v0 = (y + 0.5) / static_cast<double>(_resolution.y);

            for (unsigned x = 0; x < _resolution.x; ++x) {
                const double u0 = (x + 0.5) / static_cast<double>(_resolution.x);

                for (unsigned c = 0; c < render.antialiasing.samples; ++c) {
                    double du = u0 + jitterX(rng) * (1.0 / _resolution.x);
                    double dv = v0 + jitterY(rng) * (1.0 / _resolution.y);
                    du = std::clamp(du, 0.0, 1.0);
                    dv = std::clamp(dv, 0.0, 1.0);
                    generateRay(du, dv, cameraRay);

                    const LightSample sample = getRayColor(cameraRay, shapes, lights, render, 0, rng);
                    const double L = 0.2126 * sample.radiance._x +
                        0.7152 * sample.radiance._y + 0.0722 * sample.radiance._z;
                    const double w = sample.isDelta ? L : L / std::max(sample.pdf, EPSILON);
                    restirGrid[y][x].add(sample, w, rng);
                }
            }

            const unsigned done = linesDone.fetch_add(1) + 1;
            if (done % 10 == 0 || done == _resolution.y) {
                const std::lock_guard<std::mutex> lock(progressBarMutex);
                logger::progress_bar(1.0f, static_cast<float>(done) / static_cast<float>(_resolution.y));
            }
        }
    };

    for (size_t i = 0; i < nproc; ++i) {
        threads.emplace_back(sparseWorker, i);
    }
    for (auto &t : threads) {
        t.join();
    }

    // TODO: restir spatial merge

    // image generation
    std::ofstream ppm(render.output.file);

    if (!ppm) {
        throw exception::Error("Camera", "Unable to open output file.");
    }
    ppm << "P3\n" << _resolution.x << " " << _resolution.y << "\n255\n";
    for (unsigned y = 0; y < _resolution.y; ++y) {
        for (unsigned x = 0; x < _resolution.x; ++x) {
            math::RGBColor pixel = restirGrid[y][x].estimate();
            ppm << getPixelColor(pixel._x, render.lighting.gamma) << ' '
                << getPixelColor(pixel._y, render.lighting.gamma) << ' '
                << getPixelColor(pixel._z, render.lighting.gamma) << '\n';
        }        
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

static const math::Vector3D applyRotation(const math::Vector3D &dir, const math::Vector3D &rot)
{
    const double cosX = std::cos(rot._x), sinX = std::sin(rot._x);
    const double cosY = std::cos(rot._y), sinY = std::sin(rot._y);
    const double cosZ = std::cos(rot._z), sinZ = std::sin(rot._z);

    const double m00 = cosY * cosZ;
    const double m01 = cosZ * sinX * sinY - sinZ * cosX;
    const double m02 = cosZ * cosX * sinY + sinZ * sinX;
    const double m10 = cosY * sinZ;
    const double m11 = sinZ * sinX * sinY + cosZ * cosX;
    const double m12 = sinZ * cosX * sinY - cosZ * sinX;
    const double m20 = -sinY;
    const double m21 = cosY * sinX;
    const double m22 = cosY * cosX;

    return math::Vector3D(
        m00 * dir._x + m01 * dir._y + m02 * dir._z,
        m10 * dir._x + m11 * dir._y + m12 * dir._z,
        m20 * dir._x + m21 * dir._y + m22 * dir._z
    );
}

void raytracer::Camera::generateRay(double u, double v, math::Ray &cameraRay) const noexcept
{
    cameraRay._origin = _position;

    const double aspect_ratio = static_cast<double>(_resolution.x) / static_cast<double>(_resolution.y);
    const double fov_adjustment = std::tan((_fov * M_PI / 180.0) / 2.0);

    cameraRay._dir._x = (2.0 * u - 1.0) * aspect_ratio * fov_adjustment;
    cameraRay._dir._y = (1.0 - 2.0 * v) * fov_adjustment;
    cameraRay._dir._z = 1.0;
    cameraRay._dir = cameraRay._dir.normalize();
    cameraRay._dir = applyRotation(cameraRay._dir, _rotation).normalize();
}

// clang-format on
