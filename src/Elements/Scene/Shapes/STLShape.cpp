/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#include <ranges>
#include <future>
#include <thread>
#include <numeric>
#include <algorithm>
#include "Error.hpp"
#include "Logger.hpp"
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

void raytracer::shape::STLShape::_readVertex(Vertex &vertex)
{
    _file.read(reinterpret_cast<char *>(&vertex._x), sizeof(float));
    _checkRead(sizeof(float));
    _file.read(reinterpret_cast<char *>(&vertex._y), sizeof(float));
    _checkRead(sizeof(float));
    _file.read(reinterpret_cast<char *>(&vertex._z), sizeof(float));
    _checkRead(sizeof(float));
}

void raytracer::shape::STLShape::_getTriangles()
{
    for (uint32_t i = 0; i < _n_triangles; ++i) {
        Vertex normal{}, v1{}, v2{}, v3{};
        uint16_t control;
        _readVertex(normal);
        _readVertex(v1);
        _readVertex(v2);
        _readVertex(v3);
        _file.read(reinterpret_cast<char *>(&control), sizeof(uint16_t));
        _checkRead(sizeof(uint16_t));
        _triangles.emplace_back(normal, v1, v2, v3);
    }
    if (_n_triangles != _triangles.size()) {
        throw exception::Error("raytracer::shape::STLShape::_getTriangles", "Triangle count does not match triangle count");
    }
}

void raytracer::shape::STLShape::_checkRead(const std::streamsize size) const
{
    if (!_file || _file.gcount() != size) {
        throw exception::Error("raytracer::shape::STLShape::_checkRead", "Could not read ",
            std::to_string(size), " bytes from file ",  std::string(_filename));
    }
}

void raytracer::shape::STLShape::_computeMinMax(const std::size_t chunk_size, const std::size_t t, std::mutex &mutex)
{
    const std::size_t start = t * chunk_size;
    const std::size_t end = std::min(start + chunk_size, _triangles.size());
    float local_min_x = FLT_MAX, local_min_y = FLT_MAX, local_min_z = FLT_MAX;
    float local_max_x = -FLT_MAX, local_max_y = -FLT_MAX, local_max_z = -FLT_MAX;

    for (std::size_t i = start; i < end; ++i) {
        for (const Vertex verts[3] = {_triangles[i]._v1, _triangles[i]._v2, _triangles[i]._v3};
            const auto &[_x, _y, _z] : verts) {
            local_min_x = std::min(local_min_x, _x);
            local_max_x = std::max(local_max_x, _x);
            local_min_y = std::min(local_min_y, _y);
            local_max_y = std::max(local_max_y, _y);
            local_min_z = std::min(local_min_z, _z);
            local_max_z = std::max(local_max_z, _z);
        }
    }

    std::lock_guard lock(mutex);
    _min_x = std::min(_min_x, local_min_x);
    _max_x = std::max(_max_x, local_max_x);
    _min_y = std::min(_min_y, local_min_y);
    _max_y = std::max(_max_y, local_max_y);
    _min_z = std::min(_min_z, local_min_z);
    _max_z = std::max(_max_z, local_max_z);
}

void raytracer::shape::STLShape::_moveTriangles(const std::size_t chunk_size, const std::size_t t)
{
    const std::size_t start = t * chunk_size;
    const std::size_t end = std::min(start + chunk_size, _triangles.size());

    const float rx = static_cast<float>(_rotation._x) * DEG2RAD;
    const float ry = static_cast<float>(_rotation._y) * DEG2RAD;
    const float rz = static_cast<float>(_rotation._z) * DEG2RAD;

    const auto cx = std::cos(rx), sx = std::sin(rx);
    const auto cy = std::cos(ry), sy = std::sin(ry);
    const auto cz = std::cos(rz), sz = std::sin(rz);

    for (std::size_t i = start; i < end; ++i) {
        Triangle &tri = _triangles[i];
        for (Vertex *verts[3] = {&tri._v1, &tri._v2, &tri._v3};
            auto *v : verts) {
            float x = v->_x - _center_x;
            float y = v->_y - _center_y;
            float z = v->_z - _center_z;

            const float y1 = cy * y - sy * z;
            const float z1 = sy * y + cy * z;
            y = y1; z = z1;

            const float x1 = cz * x + sz * z;
            const float z2 = -sz * x + cz * z;
            x = x1; z = z2;

            const float x2 = cx * x - sx * y;
            const float y2 = sx * x + cx * y;
            x = x2; y = y2;

            v->_x = (x + static_cast<float>(_origin._x)) * _scale;
            v->_y = (y + static_cast<float>(_origin._y)) * _scale;
            v->_z = (z + static_cast<float>(_origin._z)) * _scale;
            }
        float nx = tri._vec._x;
        float ny = tri._vec._y;
        float nz = tri._vec._z;

        const float ny1 = cy * ny - sy * nz;
        const float nz1 = sy * ny + cy * nz;
        ny = ny1; nz = nz1;

        const float nx1 = cz * nx + sz * nz;
        const float nz2 = -sz * nx + cz * nz;
        nx = nx1; nz = nz2;

        const float nx2 = cx * nx - sx * ny;
        const float ny2 = sx * nx + cx * ny;
        nx = nx2; ny = ny2;

        if (const float len = std::sqrt(nx * nx + ny * ny + nz * nz); len > 0.0f) {
            tri._vec._x = nx / len;
            tri._vec._y = ny / len;
            tri._vec._z = nz / len;
        } else {
            tri._vec._x = nx;
            tri._vec._y = ny;
            tri._vec._z = nz;
        }
    }
}

void raytracer::shape::STLShape::_centerSTL()
{

    std::mutex mutex;
    std::vector<std::thread> threads;
    const std::size_t num_threads = std::thread::hardware_concurrency();

    if (num_threads <= 0) {
        throw exception::Error("raytracer::shape::STLShape::_centerSTL", "Could not create threads");
    }

    const std::size_t chunk_size = (_triangles.size() + num_threads - 1) / num_threads;

    threads.reserve(num_threads);
    for (std::size_t t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, chunk_size, t, &mutex] {
            _computeMinMax(chunk_size, t, mutex);
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
    threads.clear();

    _center_x = (_max_x + _min_x) / 2.0f;
    _center_y = (_max_y + _min_y) / 2.0f;
    _center_z = (_max_z + _min_z) / 2.0f;

    for (std::size_t t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, chunk_size, t] {
            _moveTriangles(chunk_size, t);
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
}

raytracer::shape::STLShape::STLShape(const math::Point3D &origin, const math::Point3D &rotation, const char *RESTRICT filename, const float scale):
    _scale(scale), _origin(origin), _rotation(rotation), _filename(filename)
{
    _openFile();
    _countTriangles();
    _getTriangles();
    _file.close();
    _centerSTL();
    logger::debug("STL object was built: origin ", origin, ", rotation: ", rotation, ", scale: ", _scale, ", number of triangles ", _triangles.size(), ".");
}

bool raytracer::shape::STLShape::_intersectTriangle(const math::Ray &ray, const Triangle &triangle) noexcept
{
    const math::Vector3D A(triangle._v1._x, triangle._v1._y, triangle._v1._z);
    const math::Vector3D B(triangle._v2._x, triangle._v2._y, triangle._v2._z);
    const math::Vector3D C(triangle._v3._x, triangle._v3._y, triangle._v3._z);

    // if (const math::Vector3D N(triangle._vec._x, triangle._vec._y, triangle._vec._z); ray._dir.dot(N) > 0.0f) {
    //    return false;
    // }

    const math::Vector3D AB = B - A;
    const math::Vector3D AC = C - A;

    const math::Vector3D h = ray._dir.cross(AC);
    const auto a = static_cast<float>(AB.dot(h));

    if (a == 0.0f) {
        return false;
    }

    const float f = 1.0f / a;
    const math::Vector3D s = ray._origin - A;
    const float u = f * static_cast<float>(s.dot(h));
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    const math::Vector3D q = s.cross(AB);
    if (const float v = f * static_cast<float>(ray._dir.dot(q)); v < 0.0f || u + v > 1.0f) {
        return false;
    }

    return f * AC.dot(q) > 0.0f;
}

bool raytracer::shape::STLShape::intersect(const math::Ray &ray, __attribute__((unused)) math::Point3D &intPoint) const noexcept
{
    for (const auto &triangle : _triangles) {
        if (_intersectTriangle(ray, triangle)) {
            return true;
        }
    }
    return false;
}

math::Vector3D raytracer::shape::STLShape::getPosition() const
{
    return math::Vector3D(_center_x, _center_y, _center_z);
}

math::Vector3D raytracer::shape::STLShape::getNormalAt(const math::Point3D __attribute__((unused)) & point) const noexcept
{
    const auto normal = point - math::Vector3D(_center_x, _center_y, _center_z);

    return normal.normalize();
}
