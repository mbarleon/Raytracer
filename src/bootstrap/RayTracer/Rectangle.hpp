#pragma once

#include "../Maths/Point3D.hpp"
#include "../Maths/Vector3D.hpp"

namespace RayTracer {
class Rectangle3D
{
    public:
        Math::Point3D _origin;
        Math::Vector3D _bottom_side;
        Math::Vector3D _left_side;

        Rectangle3D() = default;
        Rectangle3D(const Math::Point3D &origin, const Math::Vector3D &bottom_side, const Math::Vector3D &left_side);

        Math::Point3D pointAt(double u, double v) const;

    private:
};
}// namespace RayTracer
