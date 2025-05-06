/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#include "Error.hpp"
#include "STLShape.hpp"

void raytracer::shape::STLShape::_openFile()
{
    _file = std::ifstream(_filename, std::ios::binary);

    if (!_file.is_open()) {
        throw exception::Error("raytracer::shape::STLShape::_openFile", "Could not open file ", std::string(_filename));
    }

    _file.seekg(STL_COMMENT_LENGTH, std::ios::beg);
    if (!_file) {
        throw exception::Error("raytracer::shape::STLShape::_openFile", "Could not read from file ", std::string(_filename));
    }
}

void raytracer::shape::STLShape::_countTriangles()
{
    _file.read(reinterpret_cast<char *>(&_n_triangles), sizeof(uint32_t));
    _checkRead(sizeof(uint32_t));

    try {
        _triangles.reserve(_n_triangles);
    } catch (__attribute__((unused)) const std::bad_alloc &e) {
        throw exception::Error("raytracer::shape::STLShape::_countTriangles", "Could not allocate memory for ", std::string(_filename));
    }
}

void raytracer::shape::STLShape::_getTriangles()
{
    for (uint32_t i = 0; i < _n_triangles; ++i) {
        uint16_t control;
        _Vertex vertex_array[4];

        for (auto &vert : vertex_array) {
            _file.read(reinterpret_cast<char *>(&vert), sizeof(float) * 3);
            _checkRead(sizeof(float) * 3);
        }
        _triangles.push_back(_Triangle(vertex_array));
        _file.read(reinterpret_cast<char *>(&control), sizeof(uint16_t));
        _checkRead(sizeof(uint16_t));
    }
}

void raytracer::shape::STLShape::_checkRead(const std::streamsize size) const
{
    if (!_file || _file.gcount() != size) {
        throw exception::Error("raytracer::shape::STLShape::_checkRead", "Could not read file ",  std::string(_filename));
    }
}

void raytracer::shape::STLShape::_centerSTL()
{
    for (const auto &triangle : _triangles) {
        for (const _Vertex verts[3] = {triangle._v1, triangle._v2, triangle._v3};
            const auto &[_x, _y, _z] : verts) {
            _min_x = std::min(_min_x, _x); _max_x = std::max(_max_x, _x);
            _min_y = std::min(_min_y, _y); _max_y = std::max(_max_y, _y);
            _min_z = std::min(_min_z, _z); _max_z = std::max(_max_z, _z);
        }
    }

    _center_x = (_max_x - _min_x) / 2.0f;
    _center_y = (_max_y - _min_y) / 2.0f;
    _center_z = (_max_z - _min_z) / 2.0f;

    for (auto &triangle : _triangles) {
        for (_Vertex *verts[4] = {&triangle._vec, &triangle._v1, &triangle._v2, &triangle._v3};
            auto *v : verts) {
            v->_x -= _center_x;
            v->_y -= _center_y;
            v->_z -= _center_z;
        }
    }
}

constexpr raytracer::shape::STLShape::STLShape(const char *RESTRICT filename): _filename(filename)
{
    _openFile();
    _countTriangles();
    _getTriangles();
    _file.close();
    _centerSTL();
}

bool raytracer::shape::STLShape::_intersectTriangle(const math::Ray &ray, const _Triangle &triangle) noexcept
{
    const math::Vector3D A(triangle._v1._x, triangle._v1._y, triangle._v1._z);
    const math::Vector3D B(triangle._v2._x, triangle._v2._y, triangle._v2._z);
    const math::Vector3D C(triangle._v3._x, triangle._v3._y, triangle._v3._z);

    if (const math::Vector3D N(triangle._vec._x, triangle._vec._y, triangle._vec._z); ray._dir.dot(N) >= 0.0f) {
        return false;
    }

    const math::Vector3D AB = B - A;
    const math::Vector3D AC = C - A;

    const math::Vector3D h = ray._dir.cross(AC);
    const float a = AB.dot(h);

    if (a == 0.0f) {
        return false;
    }

    const float f = 1.0f / a;
    const math::Vector3D s = ray._origin - ray._dir;
    const float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    const math::Vector3D q = s.cross(AB);
    if (const float v = f * ray._dir.dot(q); v < 0.0f || u + v > 1.0f) {
        return false;
    }

    return f * AC.dot(q) > 0.0f;
}

bool raytracer::shape::STLShape::intersect(const math::Ray &ray) const noexcept override
{
    for (const auto &triangle : _triangles) {
        if (_intersectTriangle(ray, triangle)) {
            return true;
        }
    }
    return false;
}
