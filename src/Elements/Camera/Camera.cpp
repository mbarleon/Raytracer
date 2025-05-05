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

raytracer::RGBColor computeReflection(const math::Ray &ray, const math::Intersect &hit,
    const IShapesList &shapes, int depth, const raytracer::Render &render)
{
    // 1. Vecteur réfléchi R = I - 2*(I·N)*N
    math::Vector3D I = ray._dir.normalize();
    math::Vector3D N = hit.normal;
    math::Vector3D R = I - 2 * I.dot(N) * N;

    // 2. Rayon réfléchi
    math::Ray reflRay { hit.point + N*EPSILON, R.normalize() };

    // 3. Appel récursif
    return traceRay(reflRay, shapes, depth + 1, render);
}

raytracer::RGBColor computeRefraction(const math::Ray &ray,
    const math::Intersect &hit, const IShapesList &shapes, int depth,
    const raytracer::render &render)
{
    math::Vector3D I = ray._dir.normalize();
    math::Vector3D N = hit.normal;
    const raytracer::Material &M = *hit.object->getMaterial().get();

    double n1 = 1.0, n2 = M.refractiveIndex;
    double cosI = N.dot(I);

    if (cosI > 0) {
        std::swap(n1, n2);
        N = -N;
    }

    cosI = std::abs(cosI);
    double eta = n1 / n2;
    double k = 1 - eta*eta*(1 - cosI*cosI);

    if (k < 0)
        return raytracer::RGBColor(0,0,0);

    // vecteur réfracté T = ηI + (ηcosI − √k)N
    math::Vector3D T = (I * eta + N * (eta * cosI - std::sqrt(k))).normalize();
    math::Ray refrRay { hit.point - N*EPSILON, T };

    return traceRay(refrRay, shapes, depth + 1, render);
}

raytracer::RGBColor computeLighting(const math::Point3D &P, const math::Vector3D &N,
    const math::Vector3D &V, const raytracer::Material &M, const IShapesList &shapes)
{
    raytracer::RGBColor result(0,0,0);

    for (auto& lightObj : shapes) {
        const raytracer::Material &Lm = *lightObj->getMaterial().get();
        if (Lm.emissiveIntensity <= 0.0) continue;

        // 1. Direction vers la lumière et distance²
        math::Vector3D Ld = lightObj->getPosition() - P;
        double dist2 = Ld.length() * Ld.length();
        Ld = Ld.normalize();

        math::Ray shadowRay { P + N*EPSILON, Ld };
        math::Intersect tmp;

        if (!(findClosestIntersection(shadowRay, shapes, tmp)
        && tmp.distance * tmp.distance < dist2
        && tmp.object.get() ->getMaterial()->emissiveIntensity == 0.0)) {
            // 3. Atténuation (1/d²) × intensité (candela)
            double I = Lm.emissiveIntensity / dist2;

            // composante diffuse : diffuseColor × I × max(0, N·L)
            double NdotL = std::max(0.0, N.dot(Ld));
            raytracer::RGBColor diffuse = M.diffuseColor * (I * NdotL);

            // composante spéculaire : blanc × I × (max(0,R·V)^shininess)
            math::Vector3D R = reflect(-Ld, N);
            double RdotV = std::max(0.0, R.dot(V));
            raytracer::RGBColor specular(1,1,1);
            specular = specular * (I * std::pow(RdotV, Lm.shininess));

            result = result + diffuse + specular;
        }
    }
    return result;
}

raytracer::RGBColor computeColor(const math::Intersect &hit, const math::Ray &ray,
    const IShapesList & shapes, int depth, const raytracer::Render &render)
{
    math::Vector3D viewDir = -ray._dir; // direction vers la caméra

    raytracer::RGBColor local = computeLighting(hit.point, hit.normal, viewDir,
        *hit.object->getMaterial(), shapes);

    raytracer::RGBColor reflected(0,0,0);
    if (hit.object->getMaterial()->reflectivity > 0.0)
        reflected = computeReflection(ray, hit, shapes, depth);

    raytracer::RGBColor refracted(0,0,0);
    if (hit.object->getMaterial()->transparency > 0.0)
        refracted = computeRefraction(ray, hit, shapes, depth);

    double R = hit.object->getMaterial().get()->reflectivity;
    double T = hit.object->getMaterial().get()->transparency;
    double K = std::max(0.0, 1.0 - R - T);

    return local * K + reflected * R + refracted * T;
}

bool findClosestIntersection(const math::Ray &ray, const IShapesList &shapes,
    math::Intersect &intersect)
{
    double distMin = std::numeric_limits<double>::infinity();
    bool hit = false;

    for (auto& shape : shapes) {
        math::Point3D intersectPoint;

        if (shape->intersect(ray, intersectPoint)) {
            double dist = (intersectPoint - ray._origin).length();
            if (dist < distMin) {
                distMin = dist;
                hit = true;
                intersect.object = shape;
                intersect.point = intersectPoint;
                // calcul de la normale selon la forme (ex: sphère)
                intersect.normal = shape->getNormalAt(intersectPoint);
                intersect.distance = dist;
            }
        }
    }
    return hit;
}

raytracer::RGBColor traceRay(const math::Ray &ray, const IShapesList &shapes,
    int depth, const raytracer::Render &render)
{
    math::Intersect hit;

    if (depth > render.maxDepth || !findClosestIntersection(ray, shapes, hit))
        return raytracer::RGBColor(0,0,0);

    const raytracer::shape::IShape &object = *hit.object.get();
    const raytracer::Material &material = *object.getMaterial().get();

    // objet lumineux
    if (material.emissiveIntensity > 0.0)
        return object.getColor() * material.emissiveIntensity;

    // objet simple, réflexions, réfractions...
    return computeColor(hit, ray, shapes, depth, render);
}

void raytracer::Camera::render(const IShapesList &shapes, const Render &render) const noexcept
{
    std::ofstream ppm(render.output.file);
    ppm << "P3\n" << _resolution.x << " " << _resolution.y << "\n255\n";

    math::Ray cameraRay;

    for (unsigned y = 0; y < _resolution.y; ++y) {
        for (unsigned x = 0; x < _resolution.x; ++x) {
            generateRay(x, y, cameraRay);

            RGBColor pixel = traceRay(cameraRay, shapes, 0, render);

            // conversion de l'interval [0, 1] vers [0, 255] pour l'affichage
            int R = std::min(255, std::max(0, int(pixel.r * 255)));
            int G = std::min(255, std::max(0, int(pixel.g * 255)));
            int B = std::min(255, std::max(0, int(pixel.b * 255)));

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
//     double cosX = std::cos(pitch), sinX = std::sin(pitch);
//     double cosY = std::cos(yaw),   sinY = std::sin(yaw);
//     double cosZ = std::cos(roll),  sinZ = std::sin(roll);

//     double m00 = cosY * cosZ;
//     double m01 = cosZ * sinX * sinY - sinZ * cosX;
//     double m02 = cosZ * cosX * sinY + sinZ * sinX;

//     double m10 = cosY * sinZ;
//     double m11 = sinZ * sinX * sinY + cosZ * cosX;
//     double m12 = sinZ * cosX * sinY - cosZ * sinX;

//     double m20 = -sinY;
//     double m21 = cosY * sinX;
//     double m22 = cosY * cosX;

//     return math::Vector3D(
//         m00 * dir._x + m01 * dir._y + m02 * dir._z,
//         m10 * dir._x + m11 * dir._y + m12 * dir._z,
//         m20 * dir._x + m21 * dir._y + m22 * dir._z
//     );
// }

void raytracer::Camera::generateRay(double u, double v, math::Ray &cameraRay) const noexcept
{
    double aspect_ratio = static_cast<double>(_resolution.x) / static_cast<double>(_resolution.y);
    double fov_adjustment = std::tan((_fov * M_PI / 180.0) / 2.0);

    cameraRay._dir._x = (2.0 * u - 1.0) * aspect_ratio * fov_adjustment;
    cameraRay._dir._y = (1.0 - 2.0 * v) * fov_adjustment;
    cameraRay._dir._z = -1.0;
    cameraRay._dir = cameraRay._dir.normalize();
    // cameraRay._dir = applyRotation(cameraRay._dir, _rotation).normalize();
}

// clang-format on
