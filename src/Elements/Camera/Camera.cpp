/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera.cpp
*/

#include "Camera.hpp"
#include "Logger.hpp"
#include <cmath>
#include <fstream>
#include <random>

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

const raytracer::RGBColor raytracer::computeReflection(const ImagePixel &pixel,
    const math::Ray &incoming, const math::Intersect &intersect, const IShapesList &shapes,
    const IShapesList &lights, unsigned int depth, const raytracer::Render &render,
    std::vector<std::vector<ReSTIR_Tank>> &tank_grid)
{
    const math::Vector3D I = incoming._dir.normalize();
    const math::Vector3D N = intersect.normal;

    // R = I − 2*(I·N)*N
    const math::Vector3D R = I - N * (2.0 * I.dot(N));
    const math::Ray reflectedRay = offsetRay(intersect.point, N, R);

    return traceRay(pixel, reflectedRay, shapes, lights, depth + 1, render, tank_grid);
}

const raytracer::RGBColor raytracer::computeRefraction(const ImagePixel &pixel,
    const math::Ray &incoming, const math::Intersect &intersect, const IShapesList &shapes,
    const IShapesList &lights, unsigned int depth, const raytracer::Render &render,
    std::vector<std::vector<ReSTIR_Tank>> &tank_grid)
{
    const math::Vector3D I = incoming._dir.normalize();
    math::Vector3D N = intersect.normal;
    const auto &M = *intersect.object->getMaterial();

    double n1 = 1.0;
    double n2 = M.refractiveIndex;
    double cosI = I.dot(N);
    if (cosI > 0) {
        std::swap(n1, n2);
        N = N * -1.0;
    }
    cosI = std::abs(cosI);

    const double eta = n1 / n2;
    const double k = 1 - eta * eta * (1 - cosI * cosI);

    if (k < 0) {
        return RGBColor(0,0,0);
    }

    // refracted ray T = ηI + (ηcosI − √k)N
    const math::Vector3D T = (I * eta + N * (eta * cosI - std::sqrt(k))).normalize();
    const math::Ray refractedRay = offsetRay(intersect.point, N * -1.0, T);

    return traceRay(pixel, refractedRay, shapes, lights, depth + 1, render, tank_grid);
}

const raytracer::RGBColor raytracer::computeAmbientOcclusion(const math::Intersect &intersect,
    unsigned int aoSamples, const IShapesList &shapes, const IShapesList &lights)
{
    int unoccluded = 0;
    const uint32_t seed = static_cast<uint32_t>(intersect.point._x * 73856093u) ^
        static_cast<uint32_t>(intersect.point._y * 19349663u) ^
        static_cast<uint32_t>(intersect.point._z * 83492791u);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (unsigned int i = 0; i < aoSamples; ++i) {
        // pick random direction
        const double u1 = dist(gen);
        const double u2 = dist(gen);
        const double r = std::sqrt(1 - u1*u1), phi = 2 * M_PI * u2;
        const math::Vector3D dir (r * std::cos(phi), r * std::sin(phi), u1);
        // (transformer dir via base locale, omis ici pour la concision)
        const math::Ray aoRay = offsetRay(intersect.point, intersect.normal, dir);

        math::Intersect tmp;
        if (!findClosestIntersection(aoRay, shapes, lights, tmp)) {
            ++unoccluded;
        }
    }
    const double visibility = double(unoccluded) / aoSamples;
    return RGBColor(visibility, visibility, visibility);
}

const raytracer::RGBColor raytracer::computeDirectLighting(const ImagePixel &pixel,
    const math::Ray &ray, const math::Intersect &intersect,
    const IShapesList &shapes, const IShapesList &lights, const raytracer::Render &render,
    std::vector<std::vector<ReSTIR_Tank>> &tank_grid)
{
    const uint32_t seed = static_cast<uint32_t>(pixel.coordinates.x * 73856093u) ^
        static_cast<uint32_t>(pixel.coordinates.y * 19349663u);
    std::mt19937 gen(seed);
    ReSTIR_Tank tank;

    const long unsigned int lightCount = lights.size();

    if (lightCount == 0)
        return RGBColor(0,0,0);

    const int N = render.occlusion.restir.spatial.samples;
    std::uniform_int_distribution<unsigned long int> lightDist(0, lightCount - 1);

    for (int i = 0; i < N; ++i) {
        // pick random light
        const auto lightObj = lights[lightDist(gen)];
        const auto &Lm = *lightObj->getMaterial();

        // direction & distance
        const math::Vector3D toLight = lightObj->getPosition() - intersect.point;
        const double dist2 = toLight.dot(toLight);
        const math::Vector3D Ld = toLight.normalize();

        // shadow ray
        math::Ray shadowRay = offsetRay(intersect.point, intersect.normal, Ld);
        math::Intersect tmp;
        if (findClosestIntersection(shadowRay, shapes, lights, tmp)
         && tmp.object->getMaterial()->emissiveIntensity == 0.0
         && tmp.distance*tmp.distance < dist2)
            continue;

        // atténuation & Phong BRDF
        const double attenuation = Lm.emissiveIntensity / (4 * M_PI * dist2);
        const double NdotL = std::max(0.0, intersect.normal.dot(Ld));

        // Lambert diffuse
        RGBColor diffuse = intersect.object->getColor()
            * (render.lighting.diffuse * attenuation * NdotL);

        // Phong specular
        const math::Vector3D V = -ray._dir.normalize();
        const math::Vector3D R = reflect(-Ld, intersect.normal).normalize();
        const double specFactor = std::pow(std::max(0.0, R.dot(V)),
            intersect.object->getMaterial()->shininess);
        const RGBColor specular = RGBColor(1,1,1)
            * (render.lighting.specular * attenuation * specFactor);

        LightSample candidate;
        candidate.radiance = diffuse + specular;
        candidate.pdf = 1.0 / double(lightCount);

        // w = Li * cosθ / pdf
        const double weight = (candidate.radiance.r + candidate.radiance.g +
            candidate.radiance.b) / 3.0 / candidate.pdf;

        tank.add(candidate, weight, gen);
    }

    // spatial fusion with neighbour in a radius of 1px
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (std::abs(dx) + std::abs(dy) == 0) {
                continue;
            }

            const int nx = pixel.coordinates.x + dx;
            const int ny = pixel.coordinates.y + dy;

            if (!(nx < 0 || ny < 0 || nx >= static_cast<int>(pixel.image.x) ||
            ny >= static_cast<int>(pixel.image.y))) {
                tank.merge(tank_grid[ny][nx], gen);
            }
        }
    }
    tank_grid[pixel.coordinates.y][pixel.coordinates.x] = tank;
    return tank.estimate();
}

bool raytracer::findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    const IShapesList &lights, math::Intersect &intersect)
{
    double distMin = std::numeric_limits<double>::infinity();
    math::Point3D intersectPoint;
    bool hit = false;

    for (const auto &shape : shapes) {
        if (shape->intersect(ray, intersectPoint)) {
            const double dist = (intersectPoint - ray._origin).length();
            if (dist < distMin) {
                distMin = dist;
                hit = true;
                intersect.object = shape;
                intersect.point = intersectPoint;
                intersect.normal = shape->getNormalAt(intersectPoint);
                intersect.distance = dist;
            }
        }
    }
    for (const auto &light : lights) {
        if (light->intersect(ray, intersectPoint)) {
            const double dist = (intersectPoint - ray._origin).length();
            if (dist < distMin) {
                distMin = dist;
                hit = true;
                intersect.object = light;
                intersect.point = intersectPoint;
                intersect.normal = light->getNormalAt(intersectPoint);
                intersect.distance = dist;
            }
        }
    }
    return hit;
}

const raytracer::RGBColor raytracer::traceRay(const ImagePixel &pixel,
    const math::Ray &ray, const IShapesList &shapes, const IShapesList &lights,
    unsigned int depth, const raytracer::Render &render,
    std::vector<std::vector<ReSTIR_Tank>> &tank_grid)
{
    if (depth > render.maxDepth) {
        return RGBColor(0,0,0);
    }

    math::Intersect intersect;

    if (!findClosestIntersection(ray, shapes, lights, intersect)) {
        return RGBColor(0,0,0);
    }

    const auto &mat = *intersect.object->getMaterial();
    const RGBColor surfaceColor = intersect.object->getColor();

    if (mat.emissiveIntensity > 0.0) {
        return surfaceColor * mat.emissiveIntensity;
    }

    const RGBColor ambient = surfaceColor * computeAmbientOcclusion(intersect,
        static_cast<int>(render.occlusion.samples * 100.0), shapes, lights) * render.lighting.ambient;

    const RGBColor direct = computeDirectLighting(pixel, ray, intersect, shapes, lights,
        render, tank_grid);

    RGBColor reflected(0,0,0);
    if (mat.reflectivity > 0.0)
        reflected = computeReflection(pixel, ray, intersect, shapes, lights, depth,
            render, tank_grid) * mat.reflectivity;

    RGBColor refracted(0,0,0);
    if (mat.transparency > 0.0)
        refracted = computeRefraction(pixel, ray, intersect, shapes, lights, depth,
            render, tank_grid) * mat.transparency;

    const double K = std::max(0.0, 1.0 - mat.reflectivity - mat.transparency);
    return ambient + (direct * K) + reflected + refracted;
}

void raytracer::Camera::render(const IShapesList &shapes, const IShapesList &lights,
    const Render &render) const noexcept
{
    std::ofstream ppm(render.output.file);
    ppm << "P3\n" << _resolution.x << " " << _resolution.y << "\n255\n";

    std::vector<std::vector<ReSTIR_Tank>> tank_grid;
    math::Ray cameraRay;
    ImagePixel camPixel;

    tank_grid.resize(_resolution.y);
    for (size_t row = 0; row < _resolution.y; ++row) {
        tank_grid[row].resize(_resolution.x);
    }
    camPixel.image = _resolution;
    for (unsigned y = 0; y < _resolution.y; ++y) {
        for (unsigned x = 0; x < _resolution.x; ++x) {
            const double u = (x + 0.5) / double(_resolution.x);
            const double v = (y + 0.5) / double(_resolution.y);
            generateRay(u, v, cameraRay);
            camPixel.coordinates = {x, y};

            RGBColor pixel = traceRay(camPixel, cameraRay, shapes, lights, 0, render, tank_grid);

            pixel.realign(1, 255);
            ppm << pixel << '\n';
        }
        logger::progress_bar(static_cast<float>(_resolution.y), static_cast<float>(y + 1));
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
