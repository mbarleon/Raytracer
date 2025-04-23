#pragma once

#include "../Maths/Point3D.hpp"
#include "../Maths/Vector3D.hpp"

namespace RayTracer {
class Ray
{
    public:
        Ray() = default;
        Ray(Math::Point3D p3, Math::Vector3D v3);

        Math::Point3D _origin;
        Math::Vector3D _direction;

    private:
};
}// namespace RayTracer
