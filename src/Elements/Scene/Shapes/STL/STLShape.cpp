/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#include "STLShape.hpp"
#include "Error.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>

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
        throw exception::Error("raytracer::shape::STLShape::_countTriangles", "Could not allocate memory for ",
            std::string(_filename));
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

        const float ux = v2._x - v1._x, uy = v2._y - v1._y, uz = v2._z - v1._z;
        const float vx = v3._x - v1._x, vy = v3._y - v1._y, vz = v3._z - v1._z;
        normal._x = uy * vz - uz * vy;
        normal._y = uz * vx - ux * vz;
        normal._z = ux * vy - uy * vx;

        if (const float len = std::sqrt(normal._x * normal._x + normal._y * normal._y + normal._z * normal._z); len > 0.0f) {
            normal._x /= len;
            normal._y /= len;
            normal._z /= len;
        }
        _triangles.emplace_back(normal, v1, v2, v3);
    }
    if (_n_triangles != _triangles.size()) {
        throw exception::Error("raytracer::shape::STLShape::_getTriangles", "Triangle count does not match triangle count");
    }
}

void raytracer::shape::STLShape::_checkRead(const std::streamsize size) const
{
    if (!_file || _file.gcount() != size) {
        throw exception::Error("raytracer::shape::STLShape::_checkRead", "Could not read ", std::to_string(size),
            " bytes from file ", std::string(_filename));
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
        for (Vertex *verts[3] = {&tri._v1, &tri._v2, &tri._v3}; auto *v : verts) {
            float x = v->_x - _center_x;
            float y = v->_y - _center_y;
            float z = v->_z - _center_z;

            const float y1 = cy * y - sy * z;
            const float z1 = sy * y + cy * z;
            y = y1;
            z = z1;

            const float x1 = cz * x + sz * z;
            const float z2 = -sz * x + cz * z;
            x = x1;
            z = z2;

            const float x2 = cx * x - sx * y;
            const float y2 = sx * x + cx * y;
            x = x2;
            y = y2;

            v->_x = (x + static_cast<float>(_origin._x)) * _scale;
            v->_y = (y + static_cast<float>(_origin._y)) * _scale;
            v->_z = (z + static_cast<float>(_origin._z)) * _scale;
        }
        float nx = tri._vec._x;
        float ny = tri._vec._y;
        float nz = tri._vec._z;

        const float ny1 = cy * ny - sy * nz;
        const float nz1 = sy * ny + cy * nz;
        ny = ny1;
        nz = nz1;

        const float nx1 = cz * nx + sz * nz;
        const float nz2 = -sz * nx + cz * nz;
        nx = nx1;
        nz = nz2;

        const float nx2 = cx * nx - sx * ny;
        const float ny2 = sx * nx + cx * ny;
        nx = nx2;
        ny = ny2;

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
        threads.emplace_back([this, chunk_size, t, &mutex] { _computeMinMax(chunk_size, t, mutex); });
    }
    for (auto &thread : threads) {
        thread.join();
    }
    threads.clear();
    _center_x = (_max_x + _min_x) / 2.0f;
    _center_y = (_max_y + _min_y) / 2.0f;
    _center_z = (_max_z + _min_z) / 2.0f;
    for (std::size_t t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, chunk_size, t] { _moveTriangles(chunk_size, t); });
    }
    for (auto &thread : threads) {
        thread.join();
    }
}

raytracer::shape::STLShape::STLShape(const math::Point3D &origin, const math::Point3D &rotation, const char *RESTRICT filename,
    const float scale)
    : _scale(scale), _origin(origin), _rotation(rotation), _filename(filename)
{
    _openFile();
    _countTriangles();
    _getTriangles();
    _file.close();
    _centerSTL();
    _buildBVH();
    logger::debug("STL object was built: origin ", origin, ", rotation: ", rotation, ", scale: ", _scale,
        ", number of triangles ", _triangles.size(), ".");
}

bool raytracer::shape::STLShape::_intersectTriangle(const math::Ray &ray, const Triangle &triangle, math::Point3D &intPoint,
    const bool cullBackFaces) noexcept
{
    const math::Vector3D A(triangle._v1._x, triangle._v1._y, triangle._v1._z);
    const math::Vector3D B(triangle._v2._x, triangle._v2._y, triangle._v2._z);
    const math::Vector3D C(triangle._v3._x, triangle._v3._y, triangle._v3._z);

    if (cullBackFaces) {
        if (const math::Vector3D N(triangle._vec._x, triangle._vec._y, triangle._vec._z); ray._dir.dot(N) > 0.0f) {
            return false;
        }
    }

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
    if (const float t = f * static_cast<float>(AC.dot(q)); t >= 0.0f) {
        intPoint = ray._origin + ray._dir * t;
        return true;
    }

    return false;
}

bool raytracer::shape::STLShape::intersect(const math::Ray &ray, math::Point3D &intPoint, const bool cullBackFaces) const noexcept
{
    if (_bvhNodes.empty()) {
        return false;
    }
    return _intersectBVH(ray, 0, intPoint, cullBackFaces);
}

math::Vector3D raytracer::shape::STLShape::getPosition() const
{
    return math::Vector3D(_center_x, _center_y, _center_z);
}

math::Vector3D raytracer::shape::STLShape::getNormalAt(const math::Point3D &point) const noexcept
{

    if (const std::optional<std::size_t> size = _findTriangleInBVH(point, 0); size.has_value()) {
        const auto &[_x, _y, _z] = _triangles[size.value()]._vec;
        return math::Vector3D(_x, _y, _z).normalize();
    }
    return math::Vector3D(_center_x, _center_y, _center_z).normalize();
}

double raytracer::shape::STLShape::getAOMaxDistance() const
{
    return 10 * _scale;
}

math::RGBColor raytracer::shape::STLShape::getColorAt(const math::Point3D __attribute__((unused)) &p) const
{
    return math::RGBColor(1);
}

void raytracer::shape::STLShape::_buildBVH()
{
    std::vector<size_t> indices(_triangles.size());
    std::iota(indices.begin(), indices.end(), 0);
    _bvhNodes.clear();
    _buildBVHRecursive(indices, 0);
}

int raytracer::shape::STLShape::_buildBVHRecursive(std::vector<size_t> &indices, const int depth)// NOLINT(*-no-recursion)
{
    BVHNode node;
    for (int i = 0; i < 3; ++i) {
        node.min[i] = FLT_MAX;
        node.max[i] = -FLT_MAX;
    }
    for (const size_t idx : indices) {
        for (const auto &tri = _triangles[idx]; const auto &[_x, _y, _z] : {tri._v1, tri._v2, tri._v3}) {
            node.min[0] = std::min(node.min[0], _x);
            node.max[0] = std::max(node.max[0], _x);
            node.min[1] = std::min(node.min[1], _y);
            node.max[1] = std::max(node.max[1], _y);
            node.min[2] = std::min(node.min[2], _z);
            node.max[2] = std::max(node.max[2], _z);
        }
    }
    if (indices.size() <= 4) {
        node.triangleIndices = indices;
        _bvhNodes.push_back(node);
        return static_cast<int>(_bvhNodes.size() - 1);
    }
    const int axis = depth % 3;
    std::ranges::sort(indices.begin(), indices.end(), [&](const size_t a, const size_t b) {
        float ca = (_triangles[a]._v1._x + _triangles[a]._v2._x + _triangles[a]._v3._x) / 3.0f;
        float cb = (_triangles[b]._v1._x + _triangles[b]._v2._x + _triangles[b]._v3._x) / 3.0f;
        if (axis == 1) {
            ca = (_triangles[a]._v1._y + _triangles[a]._v2._y + _triangles[a]._v3._y) / 3.0f;
            cb = (_triangles[b]._v1._y + _triangles[b]._v2._y + _triangles[b]._v3._y) / 3.0f;
        } else if (axis == 2) {
            ca = (_triangles[a]._v1._z + _triangles[a]._v2._z + _triangles[a]._v3._z) / 3.0f;
            cb = (_triangles[b]._v1._z + _triangles[b]._v2._z + _triangles[b]._v3._z) / 3.0f;
        }
        return ca < cb;
    });
    const size_t mid = indices.size() / 2;
    std::vector left(indices.begin(), indices.begin() + static_cast<long>(mid));
    std::vector right(indices.begin() + static_cast<long>(mid), indices.end());
    const int nodeIdx = static_cast<int>(_bvhNodes.size());
    _bvhNodes.push_back(node);
    _bvhNodes[static_cast<std::size_t>(nodeIdx)].left = _buildBVHRecursive(left, depth + 1);
    _bvhNodes[static_cast<std::size_t>(nodeIdx)].right = _buildBVHRecursive(right, depth + 1);
    return nodeIdx;
}

bool raytracer::shape::STLShape::_rayAABB(const math::Ray &ray, const float min[3], const float max[3]) noexcept
{
    float t_min = (min[0] - static_cast<float>(ray._origin._x)) / static_cast<float>(ray._dir._x);
    float t_max = (max[0] - static_cast<float>(ray._origin._x)) / static_cast<float>(ray._dir._x);
    if (t_min > t_max) {
        std::swap(t_min, t_max);
    }
    float ty_min = (min[1] - static_cast<float>(ray._origin._y)) / static_cast<float>(ray._dir._y);
    float ty_max = (max[1] - static_cast<float>(ray._origin._y)) / static_cast<float>(ray._dir._y);
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
    float tz_min = (min[2] - static_cast<float>(ray._origin._z)) / static_cast<float>(ray._dir._z);
    float tz_max = (max[2] - static_cast<float>(ray._origin._z)) / static_cast<float>(ray._dir._z);
    if (tz_min > tz_max) {
        std::swap(tz_min, tz_max);
    }
    if (t_min > tz_max || tz_min > t_max) {
        return false;
    }
    return true;
}

bool raytracer::shape::STLShape::_intersectBVH(const math::Ray &ray, const int nodeIdx,
    math::Point3D &intPoint,// NOLINT(*-no-recursion)
    const bool cullBackFaces) const
{
    if (nodeIdx < 0) {
        return false;
    }
    const auto &[min, max, left, right, triangleIndices] = _bvhNodes[static_cast<std::size_t>(nodeIdx)];
    if (!_rayAABB(ray, min, max)) {
        return false;
    }
    if (!triangleIndices.empty()) {
        return std::ranges::any_of(triangleIndices,
            [&](const size_t idx) { return _intersectTriangle(ray, _triangles[idx], intPoint, cullBackFaces); });
    }
    return _intersectBVH(ray, left, intPoint, cullBackFaces) || _intersectBVH(ray, right, intPoint, cullBackFaces);
}

bool raytracer::shape::STLShape::_pointInAABB(const math::Point3D &point, const float min[3], const float max[3]) noexcept
{
    return (point._x >= min[0] && point._x <= max[0] && point._y >= min[1] && point._y <= max[1] && point._z >= min[2]
        && point._z <= max[2]);
}

std::optional<size_t> raytracer::shape::STLShape::_findTriangleInBVH(const math::Point3D &point,
    const int nodeIdx) const// NOLINT(*-no-recursion)
{
    if (nodeIdx < 0) {
        return std::nullopt;
    }

    const auto &[min, max, left, right, triangleIndices] = _bvhNodes[static_cast<std::size_t>(nodeIdx)];

    if (!_pointInAABB(point, min, max)) {
        return std::nullopt;
    }

    for (const auto &idx : triangleIndices) {
        if (_pointInTriangle(point, _triangles[idx])) {
            return idx;
        }
    }

    if (const auto leftResult = _findTriangleInBVH(point, left)) {
        return leftResult;
    }
    return _findTriangleInBVH(point, right);
}

bool raytracer::shape::STLShape::_pointInTriangle(const math::Point3D &point, const Triangle &triangle)
{
    const math::Vector3D A(triangle._v1._x, triangle._v1._y, triangle._v1._z);
    const math::Vector3D B(triangle._v2._x, triangle._v2._y, triangle._v2._z);
    const math::Vector3D C(triangle._v3._x, triangle._v3._y, triangle._v3._z);

    const math::Vector3D v0 = B - A;
    const math::Vector3D v1 = C - A;
    const math::Vector3D v2 = point - A;

    const float dot00 = static_cast<float>(v0.dot(v0));
    const float dot01 = static_cast<float>(v0.dot(v1));
    const float dot02 = static_cast<float>(v0.dot(v2));
    const float dot11 = static_cast<float>(v1.dot(v1));
    const float dot12 = static_cast<float>(v1.dot(v2));

    const float denom = dot00 * dot11 - dot01 * dot01;
    if (denom == 0.0f) {
        return false;
    }

    const float invDenom = 1.0f / denom;
    const float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    const float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return u >= 0.0f && v >= 0.0f && u + v <= 1.0f;
}
