/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.cpp
*/

#include "Application.hpp"
#include "../Parser/Parser.hpp"
#include "CoreFactory.hpp"
#include "CoreRender.hpp"
#include "SFMLMacros.hpp"
#include <cstring>

/**
 * public
 */

raytracer::core::Application::Application(const char *RESTRICT filename, bool gui)
{
    setupConfig(filename);

    if (!gui) {
        return;
    }

    _backend = std::make_unique<Backend>();
    _ui = std::make_unique<ui::UIManager>(_backend->getWindow());

    setupPreview();
    setupUI();
}

/**
* @brief Application::run
* @details run the Raytracer Application
* @return void
*/
void raytracer::core::Application::run()
{
    while (_backend->is_running()) {
        _ui->events(_backend->event());
        _ui->update(_backend->getDeltaTime());
        _ui->render();
    }
}

raytracer::PixelBuffer raytracer::core::Application::_pixelBuffer;

/**
* private
*/

/**
* @brief Application::setupConfig
* @details setup the user configuration (sent as filename by the user)
* @param filename the (JSONC) configuration file
* @return void
*/
void raytracer::core::Application::setupConfig(const char *RESTRICT filename)
{
    const parser::JsonValue jsonc = parser::parseJsonc(filename);

    try {
        const JsonMap &root = std::get<JsonMap>(jsonc);
        const ParsedJson &render = root.at("render");
        const ParsedJson &camera = root.at("camera");
        const JsonMap &scene = std::get<JsonMap>(root.at("scene").value);
        const ParsedJson &shapes = scene.at("shapes");
        const ParsedJson &lights = scene.at("lights");

        _shapes = primitive_factory(shapes);
        _lights = light_factory(lights);
        _camera = create_camera(camera);
        _config = create_render(render);

    } catch (const std::out_of_range &e) {
        throw exception::Error("Application::setupConfig", "Invalid configuration file", e.what());
    } catch (const std::bad_alloc &e) {
        throw exception::Error("Application::setupConfig", "Camera bad allocation", e.what());
    }
}

/**
* @brief Application::setupPreview
* @details setup the preview
* @return void
*/
void raytracer::core::Application::setupPreview()
{
    _pixelBuffer = Render::toPreview(_shapes, *_camera, _config.lighting.gamma);
    _scene_preview = std::make_shared<ui::UIScenePreview>(_pixelBuffer);
    _ui->getContainer().addWidget(_scene_preview);
}

/**
* @brief Application::raytrace
* @details raytrace the scene and update the preview
* @return void
*/
void raytracer::core::Application::raytrace()
{
    static bool is_on = false;

    if (is_on) {
        _pixelBuffer = Render::toPreview(_shapes, *_camera, _config.lighting.gamma);

    } else {
        const RaytraceGrid2D grid2d = _camera->render(_shapes, _lights, _config);

        _pixelBuffer = Render::toImage(grid2d, _config.lighting.gamma);
    }

    _scene_preview->setImage(_pixelBuffer);
    is_on = !is_on;
}

/**
 * @brief Application::<static>find_extension
 * @details find the extension of a file
 * @param str the filename string to search
 * @return the file extension gefunden
 */
static const char *_find_extension(const std::string &str) noexcept
{
    const std::string::size_type idx = str.rfind('.');

    if (idx == std::string::npos) {
        return nullptr;
    }
    return str.c_str() + idx + 1;
}

/**
* @brief Application::<static>exportTo
* @details export the scene to a file, automatic file extension detection
* @param filename the file name
* @return void
*/
static void _exportTo(const std::string &filename)
{
    const char *extension = _find_extension(filename);

    if (extension == nullptr) {
        const std::string filename_png = filename + ".png";
        raytracer::logger::debug("exportTo: no extension found, defaulting to PNG.");
        raytracer::core::Render::toImageFmt(raytracer::core::Application::_pixelBuffer, filename_png.c_str());
        return;
    }

    if (strcmp(extension, "ppm") == 0) {
        raytracer::logger::debug("exportTo: exporting to PPM.");
        raytracer::core::Render::toPPM(raytracer::core::Application::_pixelBuffer, filename.c_str());
    } else if (strcmp(extension, "jpg") == 0) {
        raytracer::logger::debug("exportTo: exporting to JPG.");
        raytracer::core::Render::toImageFmt(raytracer::core::Application::_pixelBuffer, filename.c_str());
    } else if (strcmp(extension, "png") == 0) {
        raytracer::logger::debug("exportTo: exporting to PNG.");
        raytracer::core::Render::toImageFmt(raytracer::core::Application::_pixelBuffer, filename.c_str());
    } else {
        raytracer::logger::debug("exportTo: extension found but not yet supported. Defaulting to PNG.");
        raytracer::core::Render::toImageFmt(raytracer::core::Application::_pixelBuffer, filename.c_str());
    }
}

// clang-format off
void raytracer::core::Application::setupUI()
{
    ui::Container &container = _ui->getContainer();

    /**
    * * Rectangles
    */

    container.addWidget(_ui->createRectangle(Vec2(0, 0), Vec2(1920, 150)));

    /**
    * * Buttons
    */

    container.addWidget(_ui->createButton("File", Vec2(50.f, 50.f), Vec2(80.f, 50.f)));
    container.addWidget(_ui->createButton("Export", Vec2(175.f, 50.f), Vec2(105.f, 50.f), [&]() { _backend->exportScene(_exportTo); }));
    container.addWidget(_ui->createButton("Settings", Vec2(325.f, 50.f), Vec2(130.f, 50.f)));
    container.addWidget(_ui->createButton("[]", Vec2(1790.f, 50.f), Vec2(45.f, 50.f), [&]() { _backend->fullscreen(); }));
    container.addWidget(_ui->createButton("X", Vec2(1850.f, 50.f), Vec2(36.f, 50.f), [&]() { _backend->stop(); }));
    container.addWidget(_ui->createButton("RT ON", Vec2(1725.f, 275.f), Vec2(110.f, 50.f), [this]() { this->raytrace(); }));

}
// clang-format on

void raytracer::core::Application::runNoGUI()
{
    const RaytraceGrid2D grid2d = _camera->render(_shapes, _lights, _config);

    _pixelBuffer = Render::toImage(grid2d, _config.lighting.gamma);
    raytracer::core::Render::toPPM(_pixelBuffer);
}
