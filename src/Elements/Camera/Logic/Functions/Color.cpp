/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Background
*/

#include "../Pathtracer.hpp"

math::RGBColor raytracer::getBackgroundColor(const math::Vector3D &v)
{
    const math::Vector3D dir = v.normalize();
    const auto t = 0.5 * (dir._y + 1.0);

    return (1.0 - t) * math::RGBColor(1) + t * math::RGBColor(0.3, 0.5, 1.0);
}

math::RGBColor raytracer::getRayColor(const math::Ray &ray, const IShapesList &shapes,
    const ILightsList &lights, const Render &render, unsigned depth)
{
    math::Intersect intersect;

    if (depth > render.maxDepth) {
        return math::RGBColor(0);
    }
    if (!findClosestIntersection(ray, shapes, intersect, true)) {
        return getBackgroundColor(ray._dir);
    }

    const auto &mat = *intersect.object->getMaterial();
    const math::Vector3D V = -ray._dir;
    math::RGBColor Lo(0);

    // ambient
    Lo += render.lighting.ambient * intersect.object->getColor();

    // direct
    Lo += computeDirectLight(intersect, V, mat, shapes, lights, render);

    // other
    if (depth < render.maxDepth) {
        if (mat.reflectivity > 0) {
            math::Vector3D R = getReflectedVector(ray._dir, intersect.normal).normalize();
            // pondération Phong : (R·V)^shininess
            double spec = std::pow(std::max(0.0, R.dot(V)), mat.shininess);
            Lo += math::RGBColor(spec) * mat.reflectivity
                  * getRayColor(intersect.reflected, shapes, lights, render, depth+1);
        }
    }

    // indirect, restir...
    return Lo;
}
