/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#pragma once

#include "Macro.hpp"
#include "AShape.hpp"

#include <mutex>
#include <vector>
#include <cfloat>
#include <fstream>

namespace raytracer::shape {
class STLShape final: public AShape
{
    public:
        explicit STLShape(const math::Point3D &origin, const math::Point3D &rotation, const char *RESTRICT filename, float scale);

        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint) const noexcept override;

        math::Vector3D getPosition() const override;
        math::Vector3D getNormalAt(const math::Point3D __attribute__((unused)) & point) const noexcept override;

    private:
        struct Vertex {
            float _x, _y, _z;
        };

        struct Triangle {
            Vertex _vec, _v1, _v2, _v3;

            constexpr explicit Triangle(const Vertex normal, const Vertex v1, const Vertex v2, const Vertex v3) :
                _vec(normal), _v1(v1), _v2(v2), _v3(v3) {}
        };

        struct AABB {
            math::Point3D min, max;
            static AABB expand(const AABB &a, const AABB &b);
            [[nodiscard]] bool intersect(const math::Ray& ray) const noexcept;
        };

        struct BVHNode {
            AABB bounds;
            int left = -1, right = -1;
            int start = 0, count = 0;
            [[nodiscard]] bool isLeaf() const { return left == -1 && right == -1; }
        };

        void _openFile();
        void _centerSTL();
        void _getTriangles();
        void _countTriangles();
        void _readVertex(Vertex &vertex);
        void _checkRead(std::streamsize size) const;
        static AABB _computeAABB(const Triangle& tri);
        static AABB _computeTriangleAABB(const Triangle &tri);
        static float getAxis(const Vertex &v, int axis);
        static float _getVertex(const Vertex &v, int axis);
        bool _traverseBVH(int nodeIdx, const math::Ray& ray) const;
        void _moveTriangles(std::size_t chunk_size, std::size_t t);
        static double _getVectorAxis(const math::Vector3D &v, int axis);
        void _computeMinMax(std::size_t chunk_size, std::size_t t, std::mutex &mutex);
        int _buildBVH(int start, int count, unsigned int depth, unsigned int maxAsyncDepth);
        static bool _intersectTriangle(const math::Ray &ray, const Triangle &triangle) noexcept;

        const float _scale;
        std::ifstream _file;
        std::mutex _bvhMutex;
        uint32_t _n_triangles = 0;
        const math::Point3D _origin;
        std::vector<int> _triIndices;
        const math::Point3D _rotation;
        std::vector<BVHNode> _bvhNodes;
        const char *RESTRICT _filename;
        std::vector<Triangle> _triangles;
        float _center_x = 0, _center_y = 0, _center_z = 0;
        float _min_x = FLT_MAX, _min_y = FLT_MAX, _min_z = FLT_MAX;
        float _max_x = FLT_MIN, _max_y = FLT_MIN, _max_z = FLT_MIN;

        static constexpr unsigned int STL_COMMENT_LENGTH = 80;
        static constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;
};
}//namespace raytracer::shape
