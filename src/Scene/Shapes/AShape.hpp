/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShape.hpp
*/

#pragma once

#include "IShape.hpp"
#include "../Material/Material.hpp"
#include <memory>

namespace raytracer::shape {
class AShape : public IShape
{
    public:
        AShape() = default;
        ~AShape() override = default;

        //TODO: here material lib like setMAterial getMaterial ect...

    protected:
        std::shared_ptr<Material> _material;
};
}// namespace raytracer::shape
