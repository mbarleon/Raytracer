#include "Ray.hpp"

RayTracer::Ray::Ray(Math::Point3D p3, Math::Vector3D v3) : _origin(p3), _direction(v3)
{
    /* empty */
}
