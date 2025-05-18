/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus
*/

#include "Torus.hpp"
#include "Macro.hpp"
#include <complex>

raytracer::shape::Torus::Torus(const math::Point3D &center, const double majorRadius,
    const double minorRadius) : _center(center), _R(majorRadius), _r(minorRadius)
{
}

math::Vector3D raytracer::shape::Torus::getPosition() const
{
    return _center;
}

void raytracer::shape::Torus::getUV(const math::Point3D &P, double &u, double &v)
    const noexcept
{
    const math::Vector3D p = P - _center;
    const double theta = std::atan2(p._z, p._x);
    const double phi = std::atan2(p._y, std::sqrt(p._x*p._x + p._z*p._z) - _R);

    u = (theta + M_PI) / (2.0 * M_PI);
    v = (phi   + M_PI) / (2.0 * M_PI);
}

math::Vector3D raytracer::shape::Torus::getNormalAt(const math::Point3D &P) const noexcept
{
    // F = (x²+y²+z² + R² - r²)² - 4 R² (x²+z²) = 0
    const math::Vector3D p = P - _center;
    const double x = p._x, y = p._y, z = p._z;
    const double sum2 = x*x + y*y + z*z + _R*_R - _r*_r;

    // ∇F = 4 sum2 * (x, y, z) - 8 R² * (x, 0, z)
    return math::Vector3D(4.0 * sum2 * x - 8.0 * _R*_R * x,
        4.0 * sum2 * y, 4.0 * sum2 * z - 8.0 * _R*_R * z).normalize();
}

math::RGBColor raytracer::shape::Torus::getColorAt(const math::Point3D &p) const
{
    double u;
    double v;

    getUV(p, u, v);
    return _texture->value(p, u, v);
}

double raytracer::shape::Torus::getAOMaxDistance() const
{
    return 2.0 * _r;
}

bool raytracer::shape::Torus::intersect(const math::Ray &ray, math::Point3D &intPoint,
    const bool cullBackFaces) const noexcept
{
    const auto O = ray._origin - _center;
    const auto D = ray._dir;

    const double Ox = O._x, Oy = O._y, Oz = O._z;
    const double Dx = D._x, Dy = D._y, Dz = D._z;
    const double R  = _R,      r  = _r;

    const double DdotD = Dx*Dx + Dy*Dy + Dz*Dz;
    const double DdotO = Dx*Ox + Dy*Oy + Dz*Oz;
    const double OdotO = Ox*Ox + Oy*Oy + Oz*Oz;
    const double G = OdotO + R*R - r*r;

    // a t⁴ + b t³ + c t² + d t + e = 0
    const double a = DdotD * DdotD;
    const double b = 4.0 * DdotD * DdotO;
    const double c = 2.0 * DdotD * G + 4.0 * (DdotO * DdotO) + 4.0 * R*R * (Dy*Dy);
    const double d = 4.0 * DdotO * G + 8.0 * R*R * (Oy*Dy);
    const double e = G*G - 4.0 * R*R * (OdotO - Oy*Oy);

    double roots[4];
    int nRoots = solveQuartic(a, b, c, d, e, roots);
    if (nRoots == 0)
        return false;

    double bestT = std::numeric_limits<double>::infinity();
    for (int i = 0; i < nRoots; ++i) {
        const double t = roots[i];
        if (t <= EPSILON)
            continue;

        const math::Point3D P = ray._origin + ray._dir * t;
        const math::Vector3D N = getMappedNormal(P);

        if (cullBackFaces && ray._dir.dot(N) >= 0.0)
            continue;
        if (t < bestT)
            bestT = t;
    }

    if (!std::isfinite(bestT))
        return false;
    intPoint = ray._origin + ray._dir * bestT;
    return true;
}

int raytracer::shape::Torus::solveQuartic(const double A, const double B, const double C,
    const double D, const double E, double roots[4]) const noexcept
{
    const double invA = 1.0 / A;
    const double b = B * invA;
    const double c = C * invA;
    const double d = D * invA;
    const double e = E * invA;

    // x = y - b/4 => y^4 + p y^2 + q y + r = 0
    const double bb = b * b;
    const double p = -3.0/8.0 * bb + c;
    const double q =  b*b*b/8.0 - b*c/2.0 + d;
    const double r = -3.0/256.0 * bb*bb + bb*c/16.0 - b*d/4.0 + e;

    // z^3 + (p/2) z^2 + ((p^2 - 4r)/16) z - (q^2/64) = 0
    const double P = -p*p/12.0 - r;
    const double Q = -p*p*p/108.0 + p*r/3.0 - q*q/8.0;
    const double R = std::sqrt(Q*Q/4.0 + P*P*P/27.0);
    const std::complex<double> U = std::cbrt(-Q/2.0 + R);
    const std::complex<double> V = std::cbrt(-Q/2.0 - R);
    const std::complex<double> z = U + V;
    const double zz = z.real();
    const double u = zz - p/3.0;

    // y^2 ± sqrt(2u) y + (u/2 ± q/(2 sqrt(2u))) = 0
    std::vector<double> realRoots;
    if (u > -std::numeric_limits<double>::epsilon()) {
        const double alpha = std::sqrt(2.0 * u);

        // 1st pair
        const double c1 = u/2.0 + q/(2.0 * alpha);
        const double disc1 = alpha*alpha/4.0 - c1;
        if (disc1 >= 0.0) {
            const double s = std::sqrt(disc1);
            realRoots.push_back(alpha / 2.0 + s);
            realRoots.push_back(alpha / 2.0 - s);
        }

        // 2nd pair
        const double c2 = u/2.0 - q/(2.0 * alpha);
        const double disc2 = alpha*alpha/4.0 - c2;
        if (disc2 >= 0.0) {
            const double s = std::sqrt(disc2);
            realRoots.push_back(-alpha/2.0 + s);
            realRoots.push_back(-alpha/2.0 - s);
        }
    }

    // x = y - b/4
    const double shift = b / 4.0;
    int count = 0;
    for (double y : realRoots) {
        const double x = y - shift;
        bool unique = true;

        for (int i = 0; i < count; ++i) {
            if (std::fabs(roots[i] - x) < 1e-7 * std::max(1.0, std::fabs(x))) {
                unique = false;
                break;
            }
        }
        if (unique) {
            roots[count++] = x;
            if (count == 4) break;
        }
    }
    return count;
}
