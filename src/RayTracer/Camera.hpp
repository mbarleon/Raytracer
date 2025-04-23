#pragma once

#include "../Maths/Point3D.hpp"
#include "Ray.hpp"
#include "Rectangle.hpp"

namespace RayTracer {

class Camera
{
    public:
        Math::Point3D _origin;
        Rectangle3D _screen;

        Camera();
        Camera(const Math::Point3D &origin, const Rectangle3D &screen);

        Ray ray(double u, double v) const;
};
}// namespace RayTracer
