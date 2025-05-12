/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SFMLBackend.hpp
*/

#pragma once

#include "ABackend.hpp"

namespace raytracer::backend {

class SFMLBackend final : public ABackend
{
    public:
        explicit SFMLBackend();
        ~SFMLBackend() override;

    private:
        void _init();
        void _stop();
};
}// namespace raytracer::backend
