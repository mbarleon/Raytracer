/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShape
*/

#include "AShape.hpp"
#include <complex>

void raytracer::shape::AShape::setMaterial(const material::Material &material)
{
    _material = material;
}

void raytracer::shape::AShape::setShininess(const double shininess)
{
    _shininess = shininess;
}

void raytracer::shape::AShape::setTexture(const std::shared_ptr<texture::ITexture> &texture)
{
    _texture = texture;
}

void raytracer::shape::AShape::setNormalMap(const std::shared_ptr<texture::ITexture> &map, const double strength)
{
    _normalMap = map;
    _normalStrength = strength;
}

raytracer::material::Material raytracer::shape::AShape::getMaterial() const
{
    return _material;
}

double raytracer::shape::AShape::getShininess() const
{
    return _shininess;
}

math::Vector3D raytracer::shape::AShape::getMappedNormal(const math::Point3D &p) const noexcept
{
    const math::Vector3D N = getNormalAt(p);

    if (_normalMap == nullptr) {
        return N;
    }

    double u;
    double v;
    getUV(p, u, v);

    const math::Vector3D T = N.orthogonal().normalize();
    const math::Vector3D B = N.cross(T).normalize();

    const math::RGBColor rgb = _normalMap->value(p, u, v);
    math::Vector3D nt(rgb._x * 2.0 - 1.0, rgb._y * 2.0 - 1.0, rgb._z * 2.0 - 1.0);

    nt._x *= _normalStrength;
    nt._y *= _normalStrength;
    return (T * nt._x + B * nt._y + N * nt._z).normalize();
}

int raytracer::shape::AShape::solveQuartic(const double A, const double B, const double C,
    const double D, const double E, double roots[4]) noexcept
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
