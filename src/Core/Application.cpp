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

sf::Image previewRenderSimple(const std::vector<std::shared_ptr<raytracer::shape::IShape>> &shapes,
    const math::Vector2u &resolution)
{
    sf::Image image;
    image.create(resolution._x, resolution._y, sf::Color::Black);

    const math::Point3D cameraPos(0, 0, -5);// Caméra virtuelle fixe
    const double fov = 60.0;                // Champ de vision (degré)
    const double aspect = static_cast<double>(resolution._x) / resolution._y;
    const double scale = std::tan(fov * 0.5 * M_PI / 180.0);

    for (unsigned y = 0; y < resolution._y; ++y) {
        for (unsigned x = 0; x < resolution._x; ++x) {
            double px = (2 * (x + 0.5) / static_cast<double>(resolution._x) - 1) * aspect * scale;
            double py = (1 - 2 * (y + 0.5) / static_cast<double>(resolution._y)) * scale;

            math::Vector3D rayDir(px, py, 1);// vers Z+
            rayDir = rayDir.normalize();

            math::Ray ray;
            ray._origin = cameraPos;
            ray._dir = rayDir;

            double minDist = std::numeric_limits<double>::max();
            math::RGBColor color;

            for (const auto &shape : shapes) {
                math::Point3D intersection;
                if (shape->intersect(ray, intersection, true)) {
                    double dist = (intersection - ray._origin).length();
                    if (dist < minDist) {
                        minDist = dist;
                        color = shape->getColor();
                    }
                }
            }

            sf::Color pixelColor(static_cast<sf::Uint8>(std::clamp(color._x * 255.0, 0.0, 255.0)),
                static_cast<sf::Uint8>(std::clamp(color._y * 255.0, 0.0, 255.0)),
                static_cast<sf::Uint8>(std::clamp(color._z * 255.0, 0.0, 255.0)));
            image.setPixel(x, y, pixelColor);
        }
    }

    return image;
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

    const IShapesList shapes_list = primitive_factory(shapes);
    const ILightsList lights_list = light_factory(lights);
    const RenderConfig render_config = create_render(render);

    _camera = create_camera(camera);

    // const RaytraceGrid2D grid2 = _camera->render(shapes_list, lights_list, render_config);

    ui::UIManager &ui = ui::UIManager::getInstance();
    ui::Container &container = ui.getContainer();
    const auto preview = std::make_shared<ui::UIScenePreview>(previewRenderSimple(shapes_list, _camera->getResolution()));
    // const auto preview = std::make_shared<ui::UIScenePreview>(core::Render::toImage(grid2));

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
    container.addWidget(button_factory("[]", Vec2(1790.f, 50.f), Vec2(40.f, 50.f), [this]() { this->fullscreen(); }));
    container.addWidget(button_factory("X", Vec2(1850.f, 50.f), Vec2(36.f, 50.f), [this]() { this->stop(); }));

}
// clang-format on
