#include "Rectangle.hpp"

RayTracer::Rectangle3D::Rectangle3D(const Math::Point3D &origin, const Math::Vector3D &bottom_side,
    const Math::Vector3D &left_side)
    : _origin(origin), _bottom_side(bottom_side), _left_side(left_side)
{
    /* empty */
}

Math::Point3D RayTracer::Rectangle3D::pointAt(double u, double v) const
{
    return _origin + _bottom_side * u + _left_side * v;
}
