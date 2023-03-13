#include "Window.h"

Window::Window(const std::string& windowName)
    :window(sf::VideoMode(SIZE_HOR,SIZE_VERT),windowName)
{
    window.setVerticalSyncEnabled(1);
}

void Window::Update()
{
    sf::Event event;
    if (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            {
                window.close();
            }
    }
}

void Window::BeginDraw()
{
    window.clear(sf::Color(0x18,0x18,0x18));
}

void Window::Draw(const sf::Drawable& drawable)
{
    window.draw(drawable);
}

void Window::EndDraw()
{
    window.display();
}

bool Window::IsOpen() const
{
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            return false;
        return true;
}