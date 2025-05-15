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
#include "../UI/UIScenePreview.hpp"
#include "CoreFactory.hpp"
#include "CoreRender.hpp"
#include "Logger.hpp"

/**
 * public
 */

raytracer::core::Application::Application(const char *RESTRICT filename)
{
    sf::ContextSettings settings;

    settings.antialiasingLevel = 8;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    _window.create({1920, 1080}, "Raytracer", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
    _window.setFramerateLimit(60);

    ui::UIManager::getInstance().initialize(_window);

    setupPreview(filename);
    setupUI();
}

//TODO: add backend module bc not clean here
//TODO: add an observer (decorator) EventManager
void raytracer::core::Application::run()
{
    sf::Clock clock;

    while (_window.isOpen()) {

        sf::Event event{};

        while (_window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                stop();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    stop();
                }
            }

            ui::UIManager::getInstance().events(event);
        }

        const float dt = clock.restart().asSeconds();

        ui::UIManager::getInstance().update(dt);
        ui::UIManager::getInstance().render();
    }
}

/**
* private
*/

void raytracer::core::Application::stop()
{
    _window.close();
}

void raytracer::core::Application::fullscreen()
{
    static bool is_fullscreen = false;
    const sf::VideoMode mode = is_fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(1920, 1080);

    is_fullscreen = !is_fullscreen;
    _window.setSize({mode.width, mode.height});
}

void raytracer::core::Application::setupPreview(const char *RESTRICT filename)
{
    const parser::JsonValue jsonc = parser::parseJsonc(filename);
    const JsonMap &root = std::get<JsonMap>(jsonc);
    const ParsedJson &render = root.at("render");
    const ParsedJson &camera = root.at("camera");
    const JsonMap &scene = std::get<JsonMap>(root.at("scene").value);
    const ParsedJson &shapes = scene.at("shapes");
    const ParsedJson &lights = scene.at("lights");
    const ILightsList lights_list = light_factory(lights);
    const RenderConfig render_config = create_render(render);

    _shapes = primitive_factory(shapes);
    _camera = create_camera(camera);

    ui::UIManager &ui = ui::UIManager::getInstance();
    ui::Container &container = ui.getContainer();
    const auto preview = std::make_shared<ui::UIScenePreview>(Render::toPreview(_shapes, *_camera));

    container.addWidget(preview);
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
    container.addWidget(button_factory("[]", Vec2(1790.f, 50.f), Vec2(45.f, 50.f), [this]() { this->fullscreen(); }));
    container.addWidget(button_factory("X", Vec2(1850.f, 50.f), Vec2(36.f, 50.f), [this]() { this->stop(); }));

}
// clang-format on
