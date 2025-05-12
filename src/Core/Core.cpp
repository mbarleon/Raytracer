/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include "../Elements/Scene/Shapes/IShape.hpp"
#include "../Parser/Parser.hpp"
#include "CoreFactory.hpp"
#include <unordered_map>

/*
* public
*/

#include "../Elements/Render/SFML/Backend/SFMLBackend.hpp"
#include "../Elements/Render/SFML/Frontend/SFMLMenu.hpp"

/**
 * @brief Core run
 * @details Core entry-point, starts the raytracer program
 * @return void
 */
void raytracer::core::Core::run(const char *RESTRICT filename)
{
    (void) filename;

    _backend = std::make_unique<backend::SFMLBackend>();
    _frontends.insert({SFML_MENU, std::make_unique<frontend::SFMLMenu>()});

    _render();
    // const parser::JsonValue jsonc = parser::parseJsonc(filename);
    // const auto &root = std::get<JsonMap>(jsonc);
    // const auto &render = root.at("render");
    // const auto &camera = root.at("camera");
    // const auto &scene = std::get<JsonMap>(root.at("scene").value);
    // const auto &shapes = scene.at("shapes");
    // const auto &lights = scene.at("lights");
    //
    // _shapes = primitive_factory(shapes);
    // _lights = light_factory(lights);
    //
    // _render = create_render(render);
    // _camera = create_camera(camera);
    // _camera.get()->render(_shapes, _lights, *_render.get());
}

/**
* @brief _render_front
* @details render the frontends objects
* @return void
*/
void raytracer::core::Core::_render_front() const noexcept
{
    auto &window = _backend->getWindow();
    const auto &event = _backend->getEvents();

    for (const auto &[_, frontend] : _frontends) {
        if (frontend->shouldRender()) {
            frontend->render(window);
        };
        frontend->update(event);
    }
}

/**
* @brief _render
* @details clear and display the window and back <$> front communication
*/
void raytracer::core::Core::_render() const noexcept
{
    while (_backend->isOpen()) {
        _backend->clear();
        _render_front();
        _backend->display();
    }
}
