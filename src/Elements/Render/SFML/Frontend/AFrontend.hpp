/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AFrontend.hpp
*/

#pragma once

#include "IFrontend.hpp"

//TODO: helpers?
namespace raytracer::frontend {

class AFrontend : public IFrontend
{
    public:
        virtual ~AFrontend() = default;

        [[nodiscard]] bool shouldRender() const noexcept override;

    protected:
        bool _should_display = true;
};

}// namespace raytracer::frontend
