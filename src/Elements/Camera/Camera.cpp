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

raytracer::RGBColor raytracer::computeReflection(const math::Ray &ray, const math::Intersect &intersect,
    const IShapesList &shapes, unsigned int depth, const Render &render)
{
    // reflected vector R = I - 2*(I·N)*N
    const math::Vector3D I = ray._dir.normalize();
    const math::Vector3D N = intersect.normal;
    const math::Vector3D R = I - 2 * I.dot(N) * N;

    const math::Ray reflRay { intersect.point + N * EPSILON, R.normalize() };

    return traceRay(reflRay, shapes, depth + 1, render);
}

raytracer::RGBColor raytracer::computeRefraction(const math::Ray &ray,
    const math::Intersect &intersect, const IShapesList &shapes, unsigned int depth,
    const Render &render)
{
    const math::Vector3D I = ray._dir.normalize();
    math::Vector3D N = intersect.normal;
    const Material &M = *intersect.object->getMaterial().get();

    double n1 = 1.0, n2 = M.refractiveIndex;
    double cosI = N.dot(I);

    if (cosI > 0) {
        std::swap(n1, n2);
        N = -N;
    }

    cosI = std::abs(cosI);
    const double eta = n1 / n2;
    const double k = 1 - eta * eta * (1 - cosI * cosI);

    if (k < 0)
        return RGBColor(0,0,0);

    // refracted vector T = ηI + (ηcosI − √k)N
    const math::Vector3D T = (eta * I + (eta * cosI - std::sqrt(k)) * N).normalize();
    const math::Ray refrRay { intersect.point - N * EPSILON, T };

    return traceRay(refrRay, shapes, depth + 1, render);
}

inline math::Vector3D raytracer::reflect(const math::Vector3D &I, const math::Vector3D &N)
{
    // reflect R = I - 2 * (I·N) * N
    return I - N * (2.0 * I.dot(N));
}

raytracer::RGBColor raytracer::computeLighting(const math::Point3D &P, const math::Vector3D &N,
    const math::Vector3D &V, const RGBColor &surfaceColor, const Material &M, const IShapesList &shapes)
{
    RGBColor result(0,0,0);

    for (const auto &light : shapes) {
        const Material &Lm = *light->getMaterial().get();

        if (Lm.emissiveIntensity <= 0.0)
            continue;

        math::Vector3D Ld = light->getPosition() - P;
        const double dist2 = Ld.length() * Ld.length();
        Ld = Ld.normalize();

        const math::Ray shadowRay { P + N * EPSILON, Ld };
        math::Intersect tmp;
        bool blocked = findClosestIntersection(shadowRay, shapes, tmp)
        && tmp.distance * tmp.distance < dist2
        && tmp.object.get()->getMaterial().get()->emissiveIntensity == 0.0;

        if (blocked)
            continue;

        // attenuation (1/d²) × intensity (lux)
        const double I = Lm.emissiveIntensity / (4.0 * M_PI * dist2);

        // diffuse component : diffuseColor × I × max(0, N·L)
        const double NdotL = std::max(0.0, N.dot(Ld));
        const RGBColor diffuse = surfaceColor * (I * NdotL / M_PI);

        // specular component : blanc × I × (max(0,R·V)^shininess)
        const math::Vector3D R = reflect(-Ld, N);
        const double RdotV = std::max(0.0, R.dot(V));
        RGBColor specular(1,1,1);
        specular = specular * (I * std::pow(RdotV, M.shininess));

        result = result + diffuse + specular;
    }
    return result;
}

raytracer::RGBColor raytracer::computeColor(const math::Intersect &intersect, const math::Ray &ray,
    const IShapesList & shapes, unsigned int depth, const Render &render)
{
    // turn direction to camera
    const math::Vector3D viewDir = -ray._dir;

    const RGBColor local = computeLighting(intersect.point, intersect.normal, viewDir,
        intersect.object.get()->getColor(), *intersect.object.get()->getMaterial(), shapes);

    RGBColor reflected(0,0,0);
    if (intersect.object->getMaterial()->reflectivity > 0.0)
        reflected = computeReflection(ray, intersect, shapes, depth, render);

    RGBColor refracted(0,0,0);
    if (intersect.object->getMaterial()->transparency > 0.0)
        refracted = computeRefraction(ray, intersect, shapes, depth, render);

    const double R = intersect.object->getMaterial().get()->reflectivity;
    const double T = intersect.object->getMaterial().get()->transparency;
    const double K = std::max(0.0, 1.0 - R - T);

    return local * K + reflected * R + refracted * T;
}

bool raytracer::findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect)
{
    double distMin = std::numeric_limits<double>::infinity();
    math::Point3D intersectPoint;
    bool hit = false;

    for (auto &shape : shapes) {
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
    return hit;
}

raytracer::RGBColor raytracer::traceRay(const math::Ray &ray, const IShapesList &shapes,
    unsigned int depth, const Render &render)
{
    math::Intersect intersect;

    if (depth > render.maxDepth || !findClosestIntersection(ray, shapes, intersect))
        return RGBColor(0,0,0);

    const shape::IShape &object = *intersect.object.get();
    const Material &material = *object.getMaterial().get();

    // luminous object
    if (material.emissiveIntensity > 0.0)
        return object.getColor() * material.emissiveIntensity;

    // simple object, reflections, refractions...
    return computeColor(intersect, ray, shapes, depth, render);
}

void raytracer::Camera::render(const IShapesList &shapes, const Render &render) const noexcept
{
    std::ofstream ppm(render.output.file);
    ppm << "P3\n" << _resolution.x << " " << _resolution.y << "\n255\n";

    math::Ray cameraRay;

    for (unsigned y = 0; y < _resolution.y; ++y) {
        for (unsigned x = 0; x < _resolution.x; ++x) {
            const double u = (x + 0.5) / double(_resolution.x);
            const double v = (y + 0.5) / double(_resolution.y);
            generateRay(u, v, cameraRay);

            RGBColor pixel = traceRay(cameraRay, shapes, 0, render);

            pixel.realign(1, 255);
            ppm << pixel << '\n';
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////

// static math::Vector3D applyRotation(const math::Vector3D &dir, const math::Vector3D &rot)
// {
//     const double pitch = rot._x, yaw = rot._y, roll = rot._z;
//     const double cosX = std::cos(pitch), sinX = std::sin(pitch);
//     const double cosY = std::cos(yaw),   sinY = std::sin(yaw);
//     const double cosZ = std::cos(roll),  sinZ = std::sin(roll);

//     const double m00 = cosY * cosZ;
//     const double m01 = cosZ * sinX * sinY - sinZ * cosX;
//     const double m02 = cosZ * cosX * sinY + sinZ * sinX;

//     const double m10 = cosY * sinZ;
//     const double m11 = sinZ * sinX * sinY + cosZ * cosX;
//     const double m12 = sinZ * cosX * sinY - cosZ * sinX;

//     const double m20 = -sinY;
//     const double m21 = cosY * sinX;
//     const double m22 = cosY * cosX;

//     return math::Vector3D(
//         m00 * dir._x + m01 * dir._y + m02 * dir._z,
//         m10 * dir._x + m11 * dir._y + m12 * dir._z,
//         m20 * dir._x + m21 * dir._y + m22 * dir._z
//     );
// }

void raytracer::Camera::generateRay(double u, double v, math::Ray &cameraRay) const noexcept
{
    cameraRay._origin = _position;

    const double aspect_ratio = static_cast<double>(_resolution.x) / static_cast<double>(_resolution.y);
    const double fov_adjustment = std::tan((_fov * M_PI / 180.0) / 2.0);

    cameraRay._dir._x = (2.0 * u - 1.0) * aspect_ratio * fov_adjustment;
    cameraRay._dir._y = (1.0 - 2.0 * v) * fov_adjustment;
    cameraRay._dir._z = -1.0;
    cameraRay._dir = cameraRay._dir.normalize();
    // cameraRay._dir = applyRotation(cameraRay._dir, _rotation).normalize();
}

// clang-format on
