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
        constexpr explicit STLShape(const char *RESTRICT filename);

        [[nodiscard]] bool intersect(const math::Ray &ray) const noexcept override;

    private:
        struct _Vertex {
            float _x, _y, _z;
        };

        struct _Triangle {
            _Vertex _vec, _v1, _v2, _v3;

            constexpr explicit _Triangle(const _Vertex vec[4]) :
                _vec(vec[0]), _v1(vec[1]), _v2(vec[2]), _v3(vec[3]) {}
        };

        void _openFile();
        void _centerSTL();
        void _getTriangles();
        void _countTriangles();
        void _checkRead(std::streamsize size) const;
        [[nodiscard]] static bool _intersectTriangle(const math::Ray &ray, const _Triangle &triangle) noexcept;

        std::ifstream _file;
        uint32_t _n_triangles;
        const char *RESTRICT _filename;
        std::vector<_Triangle>_triangles;
        float _center_x, _center_y, _center_z;
        float _min_x, _min_y, _min_z = FLT_MAX;
        float _max_x, _max_y, _max_z = FLT_MIN;
        static constexpr unsigned int STL_COMMENT_LENGTH = 80;
};
}//namespace raytracer::shape
