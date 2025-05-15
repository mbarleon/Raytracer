/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Backend.cpp
*/

#include "Backend.hpp"
#include "../Maths/Vector2u.hpp"
#include "../UI/UIManager.hpp"

/*
 * public
 */

raytracer::core::Backend::Backend()
{
    sf::ContextSettings settings;

    settings.antialiasingLevel = RT_DEFAULT_ANTIALIASING_LEVEL;
    settings.depthBits = RT_DEFAULT_DEPTHBITS;
    settings.stencilBits = RT_DEFAULT_STENCILBITS;
    settings.majorVersion = RT_OPENGL_VERSION_MAJOR;
    settings.minorVersion = RT_OPENGL_VERSION_MINOR;

    _window.create(RT_DEFAULT_SIZE, RT_WINDOW_TITLE, RT_WINDOW_STYLE, settings);
    _window.setFramerateLimit(RT_DEFAULT_FPS);
}

bool raytracer::core::Backend::is_running() const noexcept
{
    return _window.isOpen();
}

/**
* @brief Backend::fullscreen
* @details set the window to fullscreen <|> DEAFULT_SIZE
* @return void
*/
void raytracer::core::Backend::fullscreen() noexcept
{
    static bool is_fullscreen = false;
    const sf::VideoMode mode = is_fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(RT_DEFAULT_SIZE);

    is_fullscreen = !is_fullscreen;
    _window.setSize({mode.width, mode.height});
}

/**
* @brief Backend::getDeltaTime
* @details get the delta time
* @return float (the delta time)
*/
float raytracer::core::Backend::getDeltaTime() noexcept
{
    return _clock.restart().asSeconds();
}

/**
* @brief Backend::getWindow
* @details get the window
* @return sf::RenderWindow& (the &window ref)
*/
sf::RenderWindow &raytracer::core::Backend::getWindow() noexcept
{
    return _window;
}

/**
* @brief Backend::stop
* @details stop the window
* @return void
*/
void raytracer::core::Backend::stop() noexcept
{
    this->close(_window);
}

/**
* @brief Backend::event
* @details listen to the window events
* @return (const) sf::Event (the event)
*/
const sf::Event raytracer::core::Backend::event() noexcept
{
    return event_logic(_window);
}

/**
* @brief Backend::exportScene
* @details creates a popup asking for the export scene filename
* @return void
*/
void raytracer::core::Backend::exportScene(CallbackStr callback) noexcept
{
    const math::Vector2u ws(RT_POPUP_SIZE);
    sf::RenderWindow window(RT_POPUP_SIZE, RT_WINDOW_TITLE, RT_WINDOW_STYLE);
    ui::UIManager ui(window);
    const auto &w = ui.createTextInput(
        ui::_to_vec2f({0, ws._y / 2}),
        [&](const std::string &filename) {
            callback(filename);
            window.close();
        },
        24);
    ui.getContainer().addWidget(w);
    while (window.isOpen()) {
        const sf::Event &event = event_logic(window);
        ui.events(event);
        ui.render();
    }
}

/**
* private
*/

const sf::Event raytracer::core::Backend::event_logic(sf::RenderWindow &window) noexcept
{
    sf::Event event;

    while (window.pollEvent(event)) {

        switch (event.type) {

            case sf::Event::Closed:
                this->close(window);
                break;

            case sf::Event::KeyPressed:
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Escape) {
                    this->close(window);
                }

            default:
                break;
        }
    }

    return event;
}

void raytracer::core::Backend::close(sf::RenderWindow &window) noexcept
{
    window.close();
}
