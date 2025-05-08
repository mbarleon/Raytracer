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
#include <optional>

namespace raytracer::shape {
class STLShape final: public AShape
{
    public:
        explicit STLShape(const math::Point3D &origin, const math::Point3D &rotation, const char *RESTRICT filename, float scale);

        [[nodiscard]] bool intersect(const math::Ray &ray, math::Point3D &intPoint, bool cullBackFaces) const noexcept override;

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

        struct BVHNode {
            float min[3], max[3];
            int left = -1, right = -1;
            std::vector<size_t> triangleIndices;
        };

        void _openFile();
        void _buildBVH();
        void _centerSTL();
        void _getTriangles();
        void _countTriangles();
        void _readVertex(Vertex &vertex);
        void _checkRead(std::streamsize size) const;
        void _moveTriangles(std::size_t chunk_size, std::size_t t);
        int _buildBVHRecursive(std::vector<size_t>& indices, int depth);
        void _computeMinMax(std::size_t chunk_size, std::size_t t, std::mutex &mutex);
        static bool _pointInTriangle(const math::Point3D &point, const Triangle &triangle);
        static bool _rayAABB(const math::Ray& ray, const float min[3], const float max[3]) noexcept;
        std::optional<size_t> _findTriangleInBVH(const math::Point3D &point, int nodeIdx) const;
        static bool _pointInAABB(const math::Point3D &point, const float min[3], const float max[3]) noexcept;
        bool _intersectBVH(const math::Ray& ray, int nodeIdx, math::Point3D &intPoint, bool cullBackFaces) const;
        static bool _intersectTriangle(const math::Ray &ray, const Triangle &triangle, math::Point3D &intPoint, bool cullBackFaces) noexcept;


        const float _scale;
        std::ifstream _file;
        uint32_t _n_triangles = 0;
        const math::Point3D _origin;
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
