/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"
#include "../Render/ReSTIR/Tank.hpp"
#include "Logger.hpp"
#include "Logic/Pathtracer.hpp"
#include <algorithm>
#include <atomic>
#include <mutex>
#include <random>
#include <sys/stat.h>
#include <thread>

// clang-format off

raytracer::Camera::Camera(const math::Vector2u &resolution, const math::Point3D &position,
    const math::Vector3D &rotation, const uint fov) : _resolution(resolution),
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

const math::Vector2u raytracer::Camera::getResolution() const noexcept
{
    return _resolution;
}

const raytracer::RaytraceGrid2D raytracer::Camera::render(const IShapesList &shapes, const ILightsList &lights, const RenderConfig &config) const
{
    const uint nproc = std::thread::hardware_concurrency();

    if (nproc == 0) {
        throw exception::Error("raytracer::Camera::render()", "No threads available");
    }

    std::vector<std::thread> threads;
    std::mutex progressBarMutex;

    try {
        threads.reserve(nproc);
    } catch (const std::bad_alloc &e) {
        throw exception::Error("raytracer::Camera::render()", "Thread bad allocation", e.what());
    }

    std::atomic<unsigned> linesDone(0);
    RaytraceGrid2D restirGrid(_resolution._y, std::vector<Tank>(_resolution._x));

    const auto sparseWorker = [&](const unsigned threadId) {
        std::mt19937 rng = material::getRng(threadId, _resolution._x, _resolution._y);
        math::Ray cameraRay;

        // jitter
        std::uniform_real_distribution<double> jitterX(-0.5, 0.5);
        std::uniform_real_distribution<double> jitterY(-0.5, 0.5);

        for (unsigned y = threadId; y < _resolution._y; y += nproc) {
            const double v0 = (y + 0.5) / static_cast<double>(_resolution._y);

            for (unsigned x = 0; x < _resolution._x; ++x) {
                const double u0 = (x + 0.5) / static_cast<double>(_resolution._x);

                generateRay(u0, v0, cameraRay);
                const LightSample mainSample = getRayColor(cameraRay, shapes, lights, config, 0, rng);
                if (mainSample.isDelta) {
                    restirGrid[y][x].add(mainSample, mainSample.radiance.luminance(), rng);
                    continue;
                }

                for (unsigned c = 1; c < config.antialiasing.samples; ++c) {
                    double du = u0 + jitterX(rng) * (1.0 / _resolution._x);
                    double dv = v0 + jitterY(rng) * (1.0 / _resolution._y);
                    du = std::clamp(du, 0.0, 1.0);
                    dv = std::clamp(dv, 0.0, 1.0);
                    generateRay(du, dv, cameraRay);

                    const LightSample sample = getRayColor(cameraRay, shapes, lights, config, 0, rng);
                    restirGrid[y][x].add(sample, sample.radiance.luminance() / std::max(sample.pdf, EPSILON), rng);
                }
            }

            if (const unsigned done = linesDone.fetch_add(1) + 1; done % 10 == 0 || done == _resolution._y) {
                const std::lock_guard<std::mutex> lock(progressBarMutex);
                logger::progress_bar(1.0f, static_cast<float>(done) / static_cast<float>(_resolution._y));
            }
        }
    };

    for (size_t i = 0; i < nproc; ++i) {
        threads.emplace_back(sparseWorker, i);
    }
    for (auto &t : threads) {
        t.join();
    }

    return restirGrid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

void raytracer::Camera::generateRay(const double u, const double v, math::Ray &cameraRay) const noexcept
{
    cameraRay._origin = _position;

    const double aspect_ratio = static_cast<double>(_resolution._x) / static_cast<double>(_resolution._y);
    const double fov_adjustment = std::tan((_fov * M_PI / 180.0) / 2.0);

    cameraRay._dir._x = (2.0 * u - 1.0) * aspect_ratio * fov_adjustment;
    cameraRay._dir._y = (1.0 - 2.0 * v) * fov_adjustment;
    cameraRay._dir._z = 1.0;
    cameraRay._dir = cameraRay._dir.normalize();
    cameraRay._dir = math::Vector3D::applyRotation(cameraRay._dir, _rotation).normalize();
}

// clang-format on
