/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** TangleCube
*/

#include "TangleCube.hpp"
#include "Macro.hpp"
#include <limits>
#include <cmath>
#include <algorithm>

raytracer::shape::TangleCube::TangleCube(const math::Point3D &center, const double size,
    const double thickness) : _center(center), _s(size * 0.5), _t(thickness * 0.5)
{}

math::Vector3D raytracer::shape::TangleCube::getPosition() const
{
    return _center;
}

double raytracer::shape::TangleCube::getAOMaxDistance() const
{
    return 2.0 * _t;
}

math::RGBColor raytracer::shape::TangleCube::getColorAt(const math::Point3D &P) const
{
    double u;
    double v;

    getUV(P,u,v);
    return _texture->value(P, u, v);
}

void raytracer::shape::TangleCube::getUV(const math::Point3D &P, double &u, double &v)
    const noexcept
{
    const math::Vector3D d = (P - _center).normalize();

    if (std::fabs(d._x) >= std::fabs(d._y) && std::fabs(d._x) >= std::fabs(d._z)) {
        u = (P._z - (_center._z - _s)) / (2*_s);
        v = (P._y - (_center._y - _s)) / (2*_s);
    } else if (std::fabs(d._y) >= std::fabs(d._x) && std::fabs(d._y) >= std::fabs(d._z)) {
        u = (P._x - (_center._x - _s)) / (2*_s);
        v = (P._z - (_center._z - _s)) / (2*_s);
    } else {
        u = (P._x - (_center._x - _s)) / (2*_s);
        v = (P._y - (_center._y - _s)) / (2*_s);
    }
}

math::Vector3D raytracer::shape::TangleCube::getNormalAt(const math::Point3D &P) const noexcept
{
    const math::Vector3D p = P - _center;
    const double x = p._x, y = p._y, z = p._z;
    const double t2 = _t * _t;

    return math::Vector3D(4.0 * x*x*x - 10.0 * t2 * x, 4.0 * y*y*y - 10.0 * t2 * y,
        4.0 * z*z*z - 10.0 * t2 * z).normalize();
}

bool raytracer::shape::TangleCube::intersect(const math::Ray &ray, math::Point3D &intPoint,
    bool cullBackFaces) const noexcept
{
    const math::Vector3D O = ray._origin - _center;
    const math::Vector3D D = ray._dir;
    const double Ox = O._x;
    const double Oy = O._y;
    const double Oz = O._z;
    const double Dx = D._x;
    const double Dy = D._y;
    const double Dz = D._z;
    const double t2 = _t*_t;
    const double k = 3.0 * t2 * t2;

    // F(ray(t)) = 0 → quartique At^4 + Bt^3 + Ct^2 + Dt + E = 0
    const auto coeff4 = Dx*Dx*Dx*Dx + Dy*Dy*Dy*Dy + Dz*Dz*Dz*Dz;
    const auto coeff3 = 4*(Ox*Dx*Dx*Dx + Oy*Dy*Dy*Dy + Oz*Dz*Dz*Dz);
    const auto coeff2 = 6*(Ox*Ox*Dx*Dx + Oy*Oy*Dy*Dy + Oz*Oz*Dz*Dz) - 5*t2*(Dx*Dx + Dy*Dy + Dz*Dz);
    const auto coeff1 = 4*(Ox*Ox*Ox*Dx + Oy*Oy*Oy*Dy + Oz*Oz*Oz*Dz) - 10*t2*(Ox*Dx + Oy*Dy + Oz*Dz);
    const auto coeff0 = (Ox*Ox*Ox*Ox + Oy*Oy*Oy*Oy + Oz*Oz*Oz*Oz) - 5*t2*(Ox*Ox + Oy*Oy + Oz*Oz) + k;

    double roots[4];
    const int n = solveQuartic(coeff4, coeff3, coeff2, coeff1, coeff0, roots);
    if (n == 0)
        return false;

    double bestT = std::numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i) {
        const double t = roots[i];

        if (t <= EPSILON)
            continue;
        if (const math::Point3D P = ray._origin + ray._dir * t; cullBackFaces &&
        ray._dir.dot(getMappedNormal(P)) >= 0.0)
            continue;
        bestT = std::min(bestT, t);
    }

    if (!std::isfinite(bestT))
        return false;
    intPoint = ray._origin + ray._dir * bestT;
    return true;
}
