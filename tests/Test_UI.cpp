/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Test_UI.cpp
*/

#include <../src/UI/UIManager.hpp>
#include <criterion/criterion.h>

Test(UIManager, getContainer)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    raytracer::ui::UIManager uiManager(window);
    auto &container = uiManager.getContainer();
    auto rectangle = uiManager.createRectangle({100, 100}, {200, 50});

    container.addWidget(rectangle);
    container.update(0.016f);
    container.render(window);
    container.removeWidget(rectangle);
}

Test(UIManager, createText)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    raytracer::ui::UIManager uiManager(window);
    auto text = uiManager.createText("Hello", {100, 100}, 30);

    cr_assert_not_null(text.get());
}

Test(UIManager, createTextInput)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    raytracer::ui::UIManager uiManager(window);
    auto textInput = uiManager.createTextInput({100, 100}, [](const std::string &str) {}, 30);

    cr_assert_not_null(textInput.get());
}

Test(UIManager, createButton)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    raytracer::ui::UIManager uiManager(window);
    auto button = uiManager.createButton("Click me", {100, 100}, {200, 50}, []() {}, 30);

    cr_assert_not_null(button.get());
}

Test(UIManager, createRectangle)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    raytracer::ui::UIManager uiManager(window);
    auto rectangle = uiManager.createRectangle({100, 100}, {200, 50});

    cr_assert_not_null(rectangle.get());
}
