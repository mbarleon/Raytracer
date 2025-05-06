/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** STLShape
*/

#pragma once

#include "Macro.hpp"
#include "AShape.hpp"

#include <vector>
#include <cfloat>
#include <fstream>

namespace raytracer::shape {
class STLShape final: public AShape
{
    public:
        constexpr explicit STLShape(const math::Point3D &origin, const char *RESTRICT filename);

        [[nodiscard]] bool intersect(const math::Ray &ray) const noexcept override;

    private:
        struct Vertex {
            float _x, _y, _z;
        };

        struct Triangle {
            Vertex _vec, _v1, _v2, _v3;

            constexpr explicit Triangle(const Vertex vec[4]) :
                _vec(vec[0]), _v1(vec[1]), _v2(vec[2]), _v3(vec[3]) {}
        };

        void _openFile();
        void _centerSTL();
        void _getTriangles();
        void _countTriangles();
        void _checkRead(std::streamsize size) const;
        void _moveTriangles(std::size_t chunk_size, std::size_t t);
        void _computeMinMax(std::size_t chunk_size, std::size_t t, std::mutex &mutex);
        [[nodiscard]] static bool _intersectTriangle(const math::Ray &ray, const Triangle &triangle) noexcept;

        std::ifstream _file;
        uint32_t _n_triangles = 0;
        const math::Point3D _origin;
        const char *RESTRICT _filename;
        std::vector<Triangle> _triangles;
        float _center_x = 0, _center_y = 0, _center_z = 0;
        float _min_x = FLT_MAX, _min_y = FLT_MAX, _min_z = FLT_MAX;
        float _max_x = FLT_MIN, _max_y = FLT_MIN, _max_z = FLT_MIN;
        static constexpr unsigned int STL_COMMENT_LENGTH = 80;
};
}//namespace raytracer::shape
