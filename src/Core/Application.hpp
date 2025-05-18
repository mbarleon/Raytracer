/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.hpp
*/

#pragma once

#include "../Elements/Camera/Camera.hpp"
#include "../UI/UIManager.hpp"
#include "../UI/UIScenePreview.hpp"
#include "Backend.hpp"
#include "Macro.hpp"
#include "NonCopyable.hpp"

namespace raytracer::core {

/**
* @class Application
* @brief Main Core entry-point class for the raytracer application.
*/
class Application final : public NonCopyable
{
    public:
        explicit Application(const char *RESTRICT filename, const bool gui);
        ~Application() = default;

        void run();
        void runNoGUI();

        static PixelBuffer _pixelBuffer;

    private:
        void setupUI();
        void setupPreview();
        void setupConfig(const char *RESTRICT filename);

        void raytrace();

        IShapesList _shapes;
        ILightsList _lights;
        RenderConfig _config;

        std::unique_ptr<ui::UIManager> _ui;
        std::unique_ptr<Camera> _camera;
        std::unique_ptr<Backend> _backend;
        std::shared_ptr<ui::UIScenePreview> _scene_preview;
};
}// namespace raytracer::core
