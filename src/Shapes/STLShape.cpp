/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#include <mutex>
#include <ranges>
#include <future>
#include <thread>
#include <numeric>
#include <algorithm>
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

raytracer::shape::STLShape::AABB raytracer::shape::STLShape::_computeAABB(const Triangle &tri)
{
    auto min3 = [](float a, float b, float c) {
        return std::min({a, b, c});
    };
    auto max3 = [](float a, float b, float c) {
        return std::max({a, b, c});
    };
    AABB box;
    box.min = math::Point3D(
        min3(tri._v1._x, tri._v2._x, tri._v3._x),
        min3(tri._v1._y, tri._v2._y, tri._v3._y),
        min3(tri._v1._z, tri._v2._z, tri._v3._z)
    );
    box.max = math::Point3D(
        max3(tri._v1._x, tri._v2._x, tri._v3._x),
        max3(tri._v1._y, tri._v2._y, tri._v3._y),
        max3(tri._v1._z, tri._v2._z, tri._v3._z)
    );
    return box;
}

raytracer::shape::STLShape::AABB raytracer::shape::STLShape::AABB::expand(const AABB &a, const AABB &b)
{
    return {
        math::Point3D(
            std::min(a.min._x, b.min._x),
            std::min(a.min._y, b.min._y),
            std::min(a.min._z, b.min._z)
        ),
        math::Point3D(
            std::max(a.max._x, b.max._x),
            std::max(a.max._y, b.max._y),
            std::max(a.max._z, b.max._z)
        )
    };
}

bool raytracer::shape::STLShape::AABB::intersect(const math::Ray &ray) const noexcept
{
    double t_min = (min._x - ray._origin._x) / ray._dir._x;
    double t_max = (max._x - ray._origin._x) / ray._dir._x;

    if (t_min > t_max) {
        std::swap(t_min, t_max);
    }

    double ty_min = (min._y - ray._origin._y) / ray._dir._y;
    double ty_max = (max._y - ray._origin._y) / ray._dir._y;

    if (ty_min > ty_max) {
        std::swap(ty_min, ty_max);
    }
    if (t_min > ty_max || ty_min > t_max) {
        return false;
    }
    if (ty_min > t_min) {
        t_min = ty_min;
    }
    if (ty_max < t_max) {
        t_max = ty_max;
    }

    double tz_min = (min._z - ray._origin._z) / ray._dir._z;
    double tz_max = (max._z - ray._origin._z) / ray._dir._z;

    if (tz_min > tz_max) {
        std::swap(tz_min, tz_max);
    }
    return !(t_min > tz_max || tz_min > t_max);
}

float raytracer::shape::STLShape::getAxis(const Vertex &v, const int axis)
{
    switch (axis) {
        case 0:
            return v._x;
        case 1:
            return v._y;
        case 2:
            return v._z;
        default: throw exception::Error("raytracer::shape::STLShape::getAxis", "Invalid axis index");
    }
}

int raytracer::shape::STLShape::_buildBVH(const int start, const int count, const unsigned int depth, // NOLINT(*-no-recursion)
    const unsigned int maxAsyncDepth)
{
    int nodeIdx;
    {
        static std::mutex mutex;
        std::lock_guard lock(mutex);
        nodeIdx = static_cast<int>(_bvhNodes.size());
        _bvhNodes.emplace_back();
    }

    auto &[n_bounds, n_left, n_right, n_start, n_count] = _bvhNodes[static_cast<std::size_t>(nodeIdx)];
    AABB box;
    for (std::size_t i = 0; i < static_cast<std::size_t>(count); ++i) {
        const auto s_start = static_cast<std::size_t>(start);
        box = i == 0 ? _computeAABB(_triangles[static_cast<std::size_t>(_triIndices[s_start + i])]) :
        AABB::expand(box, _computeAABB(_triangles[static_cast<std::size_t>(_triIndices[s_start + i])]));
    }
    n_bounds = box;

    if (count <= 4 || depth > 16) {
        n_start = start;
        n_count = count;
        return nodeIdx;
    }

    auto centroid = [&](const Triangle &t, const int axis) {
        return (getAxis(t._v1, axis) + getAxis(t._v2, axis) + getAxis(t._v3, axis)) / 3.0f;
    };

    int axis = 0;
    const double maxExtent = box.max._x - box.min._x;
    if (box.max._y - box.min._y > maxExtent) {
        axis = 1;
    }
    if (box.max._z - box.min._z > maxExtent) {
        axis = 2;
    }

    std::sort(_triIndices.begin() + start, _triIndices.begin() + start + count,
        [&](const int a, const int b) {
            return centroid(_triangles[static_cast<std::size_t>(a)], axis) <
                centroid(_triangles[static_cast<std::size_t>(b)], axis);
        });

    const int mid = count / 2;
    int left, right;
    if (depth < maxAsyncDepth) {
        auto leftFuture = std::async(std::launch::async, [&] {
            return _buildBVH(start, mid, depth + 1, maxAsyncDepth);
        });
        right = _buildBVH(start + mid, count - mid, depth + 1, maxAsyncDepth);
        left = leftFuture.get();
    } else {
        left = _buildBVH(start, mid, depth + 1, maxAsyncDepth);
        right = _buildBVH(start + mid, count - mid, depth + 1, maxAsyncDepth);
    }

    n_left = left;
    n_right = right;
    return nodeIdx;
}

raytracer::shape::STLShape::STLShape(const math::Point3D &origin, const char *RESTRICT filename):
    _origin(origin), _filename(filename)
{
    _openFile();
    _countTriangles();
    _getTriangles();
    _file.close();
    _centerSTL();
    _triIndices.resize(_triangles.size());
    std::iota(_triIndices.begin(), _triIndices.end(), 0);
    _buildBVH(0, static_cast<int>(_triIndices.size()), 0, std::thread::hardware_concurrency());
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

bool raytracer::shape::STLShape::_traverseBVH(const int nodeIdx, const math::Ray &ray) const // NOLINT(*-no-recursion)
{
    const BVHNode &node = _bvhNodes[static_cast<std::size_t>(nodeIdx)];

    if (!node.bounds.intersect(ray)) {
        return false;
    }
    if (node.isLeaf()) {
        for (std::size_t i = 0; i < static_cast<std::size_t>(node.count); ++i) {
            const auto s_start = static_cast<std::size_t>(node.start);
            if (const Triangle &tri = _triangles[static_cast<std::size_t>(_triIndices[s_start + i])];
                _intersectTriangle(ray, tri)) {
                return true;
            }
        }
        return false;
    }

    return _traverseBVH(node.left, ray) || _traverseBVH(node.right, ray);
}

bool raytracer::shape::STLShape::intersect(const math::Ray &ray) const noexcept
{
    return _traverseBVH(0, ray);
}
