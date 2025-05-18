/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Backend.cpp
*/

#include "Backend.hpp"
#include "../Maths/Vector2u.hpp"
#include "../UI/UIManager.hpp"
#include "SFMLMacros.hpp"

/*
 * public
 */

raytracer::core::Backend::Backend()
{
    sf::ContextSettings settings;

    settings.antiAliasingLevel = RT_DEFAULT_ANTIALIASING_LEVEL;
    settings.depthBits = RT_DEFAULT_DEPTHBITS;
    settings.stencilBits = RT_DEFAULT_STENCILBITS;
    settings.majorVersion = RT_OPENGL_VERSION_MAJOR;
    settings.minorVersion = RT_OPENGL_VERSION_MINOR;

    _window.create(sf::VideoMode(RT_DEFAULT_SIZE), RT_WINDOW_TITLE, RT_WINDOW_STYLE, sf::State::Windowed, settings);
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
    _window.setSize({mode.size});
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
const std::optional<sf::Event> raytracer::core::Backend::event() noexcept
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
    sf::RenderWindow window(sf::VideoMode(RT_POPUP_SIZE), RT_WINDOW_TITLE, RT_WINDOW_STYLE);
    ui::UIManager ui(window);
    ui::Container &c = ui.getContainer();

    c.addWidget(ui.createRectangle(ui::_to_vec2f({3, 3}), {594.f, 244.f}));
    c.addWidget(ui.createRectangle(ui::_to_vec2f({3, 3}), {594.f, 94.f}));
    c.addWidget(ui.createText("Export to:", ui::_to_vec2f({245, 40})));
    c.addWidget(ui.createTextInput(ui::_to_vec2f({20, ws._y / 2}), [&](const std::string &filename) {
        callback(filename);
        window.close();
    }));
    while (window.isOpen()) {
        ui.events(event_logic(window));
        ui.render();
    }
}

/**
* private
*/

const std::optional<sf::Event> raytracer::core::Backend::event_logic(sf::RenderWindow &window) noexcept
{
    while (auto eventOpt = window.pollEvent()) {
        const sf::Event &event = *eventOpt;

        if (event.is<sf::Event::Closed>()) {
            this->close(window);
        } else if (event.is<sf::Event::KeyPressed>()) {
            const auto *keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape) {
                this->close(window);
            }
        } else if (event.is<sf::Event::KeyReleased>()) {
            const auto *keyEvent = event.getIf<sf::Event::KeyReleased>();
            if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape) {
                this->close(window);
            }
        }

        return event;
    }

    return std::optional<sf::Event>{};
}

// const sf::Event raytracer::core::Backend::event_logic(sf::RenderWindow &window) noexcept
// {
//     sf::Event event;
//
//     while (window.pollEvent(event)) {
//
//         switch (event.type) {
//
//             case sf::Event::Closed:
//                 this->close(window);
//                 break;
//
//             case sf::Event::KeyPressed:
//             case sf::Event::KeyReleased:
//                 if (event.key.code == sf::Keyboard::Escape) {
//                     this->close(window);
//                 }
//
//             default:
//                 break;
//         }
//     }
//
//     return event;
// }

void raytracer::core::Backend::close(sf::RenderWindow &window) noexcept
{
    window.close();
}
