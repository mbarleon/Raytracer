/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Application.cpp
*/

#include "Application.hpp"
#include "../Parser/Parser.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UIManager.hpp"
#include "CoreFactory.hpp"
#include "CoreRender.hpp"
#include "SFMLMacros.hpp"

/**
 * public
 */

raytracer::core::Application::Application(const char *RESTRICT filename)
{
    _backend = std::make_unique<Backend>();
    ui::UIManager::getInstance().initialize(_backend->getWindow());

    setupConfig(filename);
    setupPreview();
    setupUI();
}

void raytracer::core::Application::run()
{
    ui::UIManager &ui = ui::UIManager::getInstance();

    while (_backend->is_running()) {
        ui.events(_backend->event());
        ui.update(_backend->getDeltaTime());
        ui.render();
    }
}

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
}

/**
* @brief Application::setupPreview
* @details setup the preview
* @return void
*/
void raytracer::core::Application::setupPreview()
{
    ui::UIManager &ui = ui::UIManager::getInstance();
    ui::Container &container = ui.getContainer();

    _scene_preview = std::make_shared<ui::UIScenePreview>(Render::toPreview(_shapes, *_camera));
    container.addWidget(_scene_preview);
}

/**
* @brief Application::raytrace
* @details raytrace the scene and update the preview
* @return void
*/
void raytracer::core::Application::raytrace()
{
    static bool is_on = false;
    PixelBuffer buffer;

    if (is_on) {

        const RaytraceGrid2D grid2d = _camera->render(_shapes, _lights, _config);

        buffer = Render::toImage(grid2d);

    } else {
        buffer = Render::toPreview(_shapes, *_camera);
    }

    _scene_preview->setImage(buffer);
    is_on = !is_on;
}

// clang-format off
void raytracer::core::Application::setupUI()
{
    ui::UIManager &ui = ui::UIManager::getInstance();
    ui::Container &container = ui.getContainer();

    /**
    * * Buttons
    */
    const auto button_factory = [&ui](const std::string &text, const Vec2 &position, const Vec2 &size, Callback callback = _clicked)
    {
        auto button = std::make_shared<ui::Button>(position, size, text, ui.getFont());
        button->setOnClick(callback);
        return button;
   };

    container.addWidget(button_factory("File", Vec2(50.f, 50.f), Vec2(80.f, 50.f)));
    container.addWidget(button_factory("Export", Vec2(175.f, 50.f), Vec2(105.f, 50.f)));
    container.addWidget(button_factory("Settings", Vec2(325.f, 50.f), Vec2(130.f, 50.f)));
    container.addWidget(button_factory("[]", Vec2(1790.f, 50.f), Vec2(45.f, 50.f), [this]() { _backend->fullscreen(); }));
    container.addWidget(button_factory("X", Vec2(1850.f, 50.f), Vec2(36.f, 50.f), [this]() { _backend->stop(); }));
    container.addWidget(button_factory("RT ON", Vec2(1725.f, 275.f), Vec2(110.f, 50.f), [this]() { this->raytrace(); }));

}
// clang-format on
