/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#include <mutex>
#include <ranges>
#include <thread>
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
        Vertex vertex_array[4];

        for (auto &vert : vertex_array) {
            _file.read(reinterpret_cast<char *>(&vert), sizeof(float) * 3);
            _checkRead(sizeof(float) * 3);
        }
        _triangles.emplace_back(vertex_array);
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

    for (std::size_t i = start; i < end; ++i) {
        Triangle &tri = _triangles[i];
        for (Vertex *verts[4] = {&tri._vec, &tri._v1, &tri._v2, &tri._v3};
            auto *v : verts) {
            v->_x += static_cast<float>(_origin._x) - _center_x;
            v->_y += static_cast<float>(_origin._y) - _center_y;
            v->_z += static_cast<float>(_origin._z) - _center_z;
        }
    }
}

void raytracer::shape::STLShape::_centerSTL()
{
    std::mutex mutex;
    std::vector<std::thread> threads;
    const std::size_t num_threads = std::thread::hardware_concurrency();
    const std::size_t chunk_size = (_triangles.size() + num_threads - 1) / num_threads;

    if (num_threads <= 0) {
        throw exception::Error("raytracer::shape::STLShape::_centerSTL", "Could not create threads");
    }

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

    _center_x = (_max_x - _min_x) / 2.0f;
    _center_y = (_max_y - _min_y) / 2.0f;
    _center_z = (_max_z - _min_z) / 2.0f;

    for (std::size_t t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, chunk_size, t] {
            _moveTriangles(chunk_size, t);
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
}

constexpr raytracer::shape::STLShape::STLShape(const math::Point3D &origin, const char *RESTRICT filename):
    _origin(origin), _filename(filename)
{
    _openFile();
    _countTriangles();
    _getTriangles();
    _file.close();
    _centerSTL();
}

bool raytracer::shape::STLShape::_intersectTriangle(const math::Ray &ray, const Triangle &triangle) noexcept
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
    const auto a = static_cast<float>(AB.dot(h));

    if (a == 0.0f) {
        return false;
    }

    const float f = 1.0f / a;
    const math::Vector3D s = ray._origin - ray._dir;
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

bool raytracer::shape::STLShape::intersect(const math::Ray &ray) const noexcept override
{
    return std::ranges::any_of(_triangles, [&](const auto &triangle) {
        return _intersectTriangle(ray, triangle);
    });
}
