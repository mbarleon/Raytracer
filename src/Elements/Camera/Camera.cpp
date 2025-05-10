/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"
#include "Logger.hpp"
#include <atomic>
#include <cmath>
#include <fstream>
#include <mutex>
#include <random>
#include <sys/stat.h>
#include <thread>

// clang-format off

raytracer::Camera::Camera(const math::Vector2u &resolution, const math::Point3D &position,
    const math::Vector3D &rotation, const uint field_of_view) : _resolution(resolution),
    _position(position), _rotation(rotation), _fov(field_of_view)
{
    logger::debug("Camera was built: resolution ", resolution, " position ", position,
        " rotation ", rotation, " fov: ", field_of_view, ".");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

void raytracer::Camera::render(const IShapesList &shapes, const IShapesList &lights,
    const Render &render) const
{
    const std::size_t nproc = std::thread::hardware_concurrency();

    if (nproc == 0) {
        throw exception::Error("raytracer::Camera::render()", "No threads available");
    }

    std::vector<std::thread> threads;
    std::vector<std::string> rows(_resolution.y);
    std::mutex progressBarMutex;

    threads.reserve(nproc);

    std::atomic<unsigned> linesDone(0);
    std::vector<std::vector<ReSTIR_Tank>> restirGrid(_resolution.y, std::vector<ReSTIR_Tank>(_resolution.x));

    const auto sparseWorker = [&](unsigned threadId) {
        std::mt19937 rng(threadId); // Each thread gets its own RNG

        for (unsigned y = threadId; y < _resolution.y; y += nproc) {
            for (unsigned x = 0; x < _resolution.x; ++x) {
                if ((x % render.occlusion.restir.spatial.radius != 0) ||
                (y % render.occlusion.restir.spatial.radius != 0))
                    continue;

                const double u = (x + 0.5) / static_cast<double>(_resolution.x);
                const double v = (y + 0.5) / static_cast<double>(_resolution.y);

                math::Ray primaryRay;
                generateRay(u, v, primaryRay);

                ImagePixel pixelInfo;
                pixelInfo.image = _resolution;
                pixelInfo.coordinates = {x, y};

                // collect light sample from path tracing
                const LightSample sample = ;//sampleLightContribution(primaryRay, pixelInfo, shapes, lights, render, rng);

                const double weight = 1.0 / std::max(sample.pdf, 1e-8);
                restirGrid[y][x].add(sample, weight, rng);
            }

            const unsigned done = linesDone.fetch_add(1) + 1;
            if (done % 10 == 0 || done == _resolution.y) {
                const std::lock_guard<std::mutex> lock(progressBarMutex);
                logger::progress_bar(0.3f, static_cast<float>(done) / static_cast<float>(_resolution.y));
            }
        }
    };

    for (size_t i = 0; i < nproc; ++i) {
        threads.emplace_back(sparseWorker, i);
    }
    for (auto &t : threads) {
        t.join();
    }

    // ReSTIR spatial propagation
    const auto spatialRadius = render.occlusion.restir.spatial.radius;
    const auto numSpatialSamples = render.occlusion.restir.spatial.samples;

    for (unsigned y = 0; y < _resolution.y; ++y) {
        for (unsigned x = 0; x < _resolution.x; ++x) {
            std::mt19937 rng(x + y * _resolution.x);

            for (unsigned i = 0; i < numSpatialSamples; ++i) {
                const int dx = static_cast<int>(x) + (rng() % (2 * spatialRadius + 1)) - spatialRadius;
                const int dy = static_cast<int>(y) + (rng() % (2 * spatialRadius + 1)) - spatialRadius;

                if (!(dx < 0 || dy < 0 || dx >= static_cast<int>(_resolution.x) ||
                dy >= static_cast<int>(_resolution.y))) {
                    restirGrid[y][x].merge(restirGrid[dy][dx], rng);
                }
            }
        }
    }

    // image generation
    linesDone.store(0);
    const auto imageWorker = [&](unsigned threadId) {
        for (unsigned y = threadId; y < _resolution.y; y += nproc) {
            std::ostringstream rowBuffer;

            for (unsigned x = 0; x < _resolution.x; ++x) {
                RGBColor pixel = restirGrid[y][x].estimate();
                pixel.realign(1.0, 255);
                rowBuffer << pixel << '\n';
            }

            rows[y] = rowBuffer.str();

            const unsigned done = linesDone.fetch_add(1) + 1;
            if (done % 10 == 0 || done == _resolution.y) {
                const std::lock_guard<std::mutex> lock(progressBarMutex);
                logger::progress_bar(1.0f, static_cast<float>(done) / static_cast<float>(_resolution.y));
            }
        }
    };

    threads.clear();
    for (size_t i = 0; i < nproc; ++i) {
        threads.emplace_back(imageWorker, i);
    }
    for (auto &t : threads) {
        t.join();
    }

    std::ofstream ppm(render.output.file);
    ppm << "P3\n" << _resolution.x << " " << _resolution.y << "\n255\n";
    for (const auto &row : rows) {
        ppm << row;
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
    cameraRay._dir._z = +1.0;
    cameraRay._dir = cameraRay._dir.normalize();
    cameraRay._dir = applyRotation(cameraRay._dir, _rotation).normalize();
}

// clang-format on
