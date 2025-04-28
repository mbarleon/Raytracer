/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShape.hpp
*/

#pragma once

#include "IShape.hpp"

namespace raytracer::shape {
class AShape : public IShape
{
    public:
        AShape() = default;
        ~AShape() override = default;

        //TODO: here material lib like setMAterial getMaterial ect...

    // protected:
        //TODO: shared_ptr of IMaterial ?
};
}// namespace raytracer::shape
