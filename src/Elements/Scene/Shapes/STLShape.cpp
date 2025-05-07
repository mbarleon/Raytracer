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
        Vertex normal, v1, v2, v3;
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

float raytracer::shape::STLShape::_getVertex(const Vertex &v, const int axis) {
    if (axis == 0) {
        return v._x;
    }
    if (axis == 1) {
        return v._y;
    }
    return v._z;
}

raytracer::shape::STLShape::AABB raytracer::shape::STLShape::_computeTriangleAABB(const Triangle &tri) {
    AABB box;
    box.min = math::Vector3D(
        std::min({tri._v1._x, tri._v2._x, tri._v3._x}),
        std::min({tri._v1._y, tri._v2._y, tri._v3._y}),
        std::min({tri._v1._z, tri._v2._z, tri._v3._z})
    );
    box.max = math::Vector3D(
        std::max({tri._v1._x, tri._v2._x, tri._v3._x}),
        std::max({tri._v1._y, tri._v2._y, tri._v3._y}),
        std::max({tri._v1._z, tri._v2._z, tri._v3._z})
    );
    return box;
}

double raytracer::shape::STLShape::_getVectorAxis(const math::Vector3D &v, const int axis)
{
    switch (axis) {
        case 0:
            return v._x;
        case 1:
            return v._y;
        case 2:
            return v._z;
        default:
            throw exception::Error("raytracer::shape::STLShape::_getVectorAxis", "Invalid axis");
    }
}

int raytracer::shape::STLShape::_buildBVH(const int start, const int count, const unsigned int depth, const unsigned int maxAsyncDepth) {
    int nodeIndex;
    {
        std::lock_guard lock(_bvhMutex);
        nodeIndex = static_cast<int>(_bvhNodes.size());
        _bvhNodes.emplace_back();
    }

    auto &node = _bvhNodes[static_cast<std::size_t>(nodeIndex)];

    AABB bounds = _computeTriangleAABB(_triangles[static_cast<std::size_t>(_triIndices[static_cast<std::size_t>(start)])]);
    for (int i = start + 1; i < count; ++i) {
        const AABB triBox = _computeTriangleAABB(_triangles[static_cast<std::size_t>(_triIndices[static_cast<std::size_t>(i)])]);
        bounds = AABB::expand(bounds, triBox);
    }
    node.bounds = bounds;

    const int triCount = count - start;

    if (triCount <= 4) {
        node.start = start;
        node.count = triCount;
        return nodeIndex;
    }

    const auto diag = bounds.max - bounds.min;
    int axis = 0;
    if (diag._y > diag._x) {
        axis = 1;
    }
    if (diag._z > _getVectorAxis(diag, axis)) {
        axis = 2;
    }

    const float split = 0.5f * static_cast<float>(_getVectorAxis(bounds.min, axis) + _getVectorAxis(bounds.max, axis));

    int mid = start;
    for (auto i = static_cast<std::size_t>(start); i < static_cast<std::size_t>(count); ++i) {
        const Triangle& tri = _triangles[static_cast<std::size_t>(_triIndices[i])];
        const float center =
            (_getVertex(tri._v1, axis) + _getVertex(tri._v2, axis) + _getVertex(tri._v3, axis)) / 3.0f;

        if (center < split) {
            std::swap(_triIndices[i], _triIndices[static_cast<std::size_t>(mid)]);
            ++mid;
        }
    }

    if (mid == start || mid == count) {
        mid = start + triCount / 2;
    }

    int left, right;
    if (depth < maxAsyncDepth) {
        auto future = std::async(std::launch::async, [&]() {
            return _buildBVH(start, mid, depth + 1, maxAsyncDepth);
        });
        right = _buildBVH(mid, count, depth + 1, maxAsyncDepth);
        left = future.get();
    } else {
        left = _buildBVH(start, mid, depth + 1, maxAsyncDepth);
        right = _buildBVH(mid, count, depth + 1, maxAsyncDepth);
    }

    node.left = left;
    node.right = right;
    return nodeIndex;
}

raytracer::shape::STLShape::STLShape(const math::Point3D &origin, const math::Point3D &rotation, const char *RESTRICT filename, const float scale):
    _scale(scale), _origin(origin), _rotation(rotation), _filename(filename)
{
    _openFile();
    _countTriangles();
    _getTriangles();
    _file.close();
    _centerSTL();
    _triIndices.resize(_triangles.size());
    std::iota(_triIndices.begin(), _triIndices.end(), 0);
    _buildBVH(0, static_cast<int>(_triIndices.size()), 0, std::thread::hardware_concurrency());
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

bool raytracer::shape::STLShape::intersect(const math::Ray &ray, __attribute__((unused)) math::Point3D &intPoint) const noexcept
{
    // return _traverseBVH(0, ray);

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
