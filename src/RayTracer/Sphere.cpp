#include "Sphere.hpp"

RayTracer::Sphere::Sphere(Math::Point3D center, double radius) : _center(center), _radius(radius)
{
    /* empty */
}

bool RayTracer::Sphere::hit(RayTracer::Ray &ray) const
{
    const Math::Vector3D oc = ray._origin - _center;
    const double a = ray._direction.dot(ray._direction);
    const double b = 2.0 * oc.dot(ray._direction);
    const double c = oc.dot(oc) - _radius * _radius;
    const double discriminant = b * b - 4 * a * c;

    return discriminant >= 0;
}
