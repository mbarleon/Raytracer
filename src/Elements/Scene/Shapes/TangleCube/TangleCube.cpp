/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** TangleCube
*/

#include "TangleCube.hpp"
#include <cmath>

using namespace raytracer::shape;
using namespace math;

TangleCube::TangleCube(const Point3D &center,
                       double width,
                       double thickness,
                       double radius,
                       unsigned maxSteps,
                       double   maxDist) noexcept
  : _center(center),
    _halfWidth(width * 0.5),
    _thickness(thickness),
    _radius(radius),
    _maxSteps(static_cast<int>(maxSteps)),
    _maxDist(maxDist)
{}

// Ray‑marching + culling
bool TangleCube::intersect(const Ray &ray,
                           Point3D &intPoint,
                           const bool cullBackFaces) const noexcept
{
    Vector3D origin    = ray._origin - _center;
    Vector3D direction = ray._dir.normalize();
    double   t         = 0.0;

    for (int i = 0; i < _maxSteps && t < _maxDist; ++i) {
        Vector3D p = origin + direction * t;
        double   d = distanceEstimator(p);
        if (d < 1e-3) {
            intPoint = ray._origin + ray._dir * t;
            if (cullBackFaces) {
                Vector3D N = getNormalAt(intPoint);
                if (direction.dot(N) >= 0.0) 
                    return false;
            }
            return true;
        }
        t += d;
    }
    return false;
}

// Gradient par différences finies
Vector3D TangleCube::getNormalAt(const Point3D &pt) const noexcept
{
    const double eps = 1e-4;
    Vector3D base = pt - _center;
    double d0 = distanceEstimator(base);
    double dx = distanceEstimator(base + Vector3D(eps,0,0)) - d0;
    double dy = distanceEstimator(base + Vector3D(0,eps,0)) - d0;
    double dz = distanceEstimator(base + Vector3D(0,0,eps)) - d0;
    return Vector3D(dx, dy, dz).normalize();
}

// UV par projection simple selon la face (approx)
void TangleCube::getUV(const Point3D &p, double &u, double &v) const noexcept
{
    Vector3D N   = getNormalAt(p);
    Vector3D loc = p - _center;
    // on projette sur le plan le plus orthogonal à N
    if (std::fabs(N._x) > 0.5) {
        // face X : proj Z,Y
        u = (loc._z / _halfWidth + 1.0) * 0.5;
        v = (loc._y / _halfWidth + 1.0) * 0.5;
    } else if (std::fabs(N._y) > 0.5) {
        // face Y : proj X,Z
        u = (loc._x / _halfWidth + 1.0) * 0.5;
        v = (loc._z / _halfWidth + 1.0) * 0.5;
    } else {
        // face Z : proj X,Y
        u = (loc._x / _halfWidth + 1.0) * 0.5;
        v = (loc._y / _halfWidth + 1.0) * 0.5;
    }
}

// couleur via texture ou nuance de gris sur Y
math::RGBColor TangleCube::getColorAt(const Point3D &p) const
{
    if (_texture) {
        double u,v; getUV(p,u,v);
        return _texture->value(p,u,v);
    }
    double h = (p._y - (_center._y - _halfWidth)) / (_halfWidth*2.0);
    return math::RGBColor(h,h,h);
}

// SDF creux arrondi = max(outerBox, -innerBox) - radius
double TangleCube::distanceEstimator(const Vector3D &p_) const noexcept
{
    // On travaille sur une copie
    Vector3D p = p_;

    Vector3D outerB(_halfWidth, _halfWidth, _halfWidth);
    Vector3D innerB(_halfWidth - _thickness, 
                    _halfWidth - _thickness, 
                    _halfWidth - _thickness);

    double dOuter = boxSDF(p, outerB);
    double dInner = boxSDF(p, innerB);

    // shell SDF
    double dShell = std::max(dOuter, -dInner);

    // arrondi final
    return dShell - _radius;
}
