#include "Camera.hpp"

RayTracer::Camera::Camera() : _origin(0, 0, 0)
{
    _screen = Rectangle3D(Math::Point3D(-1, -1, -1), Math::Vector3D(2, 0, 0), Math::Vector3D(0, 2, 0));
}

RayTracer::Camera::Camera(const Math::Point3D &origin, const Rectangle3D &screen) : _origin(origin), _screen(screen)
{
    /* empty */
}

RayTracer::Ray RayTracer::Camera::ray(double u, double v) const
{
    Math::Point3D pixel = _screen.pointAt(u, v);
    return Ray(_origin, pixel - _origin);
}
