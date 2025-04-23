#pragma once

#include "../Maths/Point3D.hpp"
#include "Ray.hpp"

namespace RayTracer {
class Sphere
{
    public:
        Math::Point3D _center;
        double _radius;
        Sphere() = default;
        Sphere(Math::Point3D center, double radius);

        bool hit(RayTracer::Ray &ray) const;
};
}// namespace RayTracer
