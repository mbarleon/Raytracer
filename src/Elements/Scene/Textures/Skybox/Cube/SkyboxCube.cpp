/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SkyboxCube
*/

#include "SkyboxCube.hpp"
#include "Error.hpp"
#include <cmath>

raytracer::texture::SkyboxCube::SkyboxCube(const std::string &filename)
{
    sf::Image src;

    if (!src.loadFromFile(filename)) {
        throw raytracer::exception::Error("SkyboxCube", "Cannot load " + filename);
    }

    const sf::Vector2u imageSize = src.getSize();

    _faceW = imageSize.x / 4;
    _faceH = imageSize.y / 2;
    extractFaces(src);
}

void raytracer::texture::SkyboxCube::extractFaces(const sf::Image &src)
{
    const SkyboxCubeCell layout[] = {
        { PosX, 2, 1 },
        { NegX, 0, 1 },
        { PosY, 1, 0 },
        { NegY, 2, 0 },
        { PosZ, 1, 1 },
        { NegZ, 3, 1 }
    };

    for (auto &c : layout) {
        sf::Image &face = _faces[c.f];

        face.create(_faceW, _faceH);
        face.copy(src, 0, 0, {
            static_cast<int>(c.cx * _faceW),
            static_cast<int>(c.cy * _faceH),
            static_cast<int>(_faceW),
            static_cast<int>(_faceH)
        }, true);
    }
}

math::RGBColor raytracer::texture::SkyboxCube::value(const math::Point3D &p,
    const double __attribute__((unused)) u, const double __attribute__((unused)) v)
    const noexcept
{
    const math::Vector3D d = p.normalize();
    const double ax = std::fabs(d._x);
    const double ay = std::fabs(d._y);
    const double az = std::fabs(d._z);

    SkyboxCubeFace face;
    double uc;
    double vc;
    if (ax >= ay && ax >= az) {
        if (d._x > 0) {
            face = PosX;
            uc = (-d._z / ax + 1.0) * 0.5;
            vc = (-d._y / ax + 1.0) * 0.5;
        } else {
            face = NegX;
            uc = ( d._z / ax + 1.0) * 0.5;
            vc = (-d._y / ax + 1.0) * 0.5;
        }
    } else if (ay >= ax && ay >= az) {
        if (d._y > 0) {
            face = PosY;
            uc = ( d._x / ay + 1.0) * 0.5;
            vc = ( d._z / ay + 1.0) * 0.5;
        } else {
            face = NegY;
            uc = ( d._x / ay + 1.0) * 0.5;
            vc = (-d._z / ay + 1.0) * 0.5;
        }
    } else {
        if (d._z > 0) {
            face = PosZ;
            uc = ( d._x / az + 1.0) * 0.5;
            vc = (-d._y / az + 1.0) * 0.5;
        } else {
            face = NegZ;
            uc = (-d._x / az + 1.0) * 0.5;
            vc = (-d._y / az + 1.0) * 0.5;
        }
    }

    if (face != NegY) {
        vc = 1.0 - vc;
    }

    const unsigned x = std::min(static_cast<unsigned>(uc * _faceW), _faceW - 1);
    const unsigned y = std::min(static_cast<unsigned>((1.0 - vc) * _faceH), _faceH - 1);

    sf::Color color = _faces[face].getPixel(x, y);
    return math::RGBColor(color.r / 255.0, color.g / 255.0, color.b / 255.0);
}
