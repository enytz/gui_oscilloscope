
#include "Button.h"

Button::Button(const sf::Font& font)
    :Bbox(sf::Vector2f(100,50)), Btext("buttom",font,20)
{
    Bbox.setOutlineColor(sf::Color::White);
    Bbox.setFillColor(sf::Color::White);
    Btext.setFillColor(sf::Color::Black);
}

bool Button::buttonIsPressed(const sf::RenderWindow& window)
{
    sf::Vector2i posMouse;
    posMouse = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && posMouse.x > Bbox.getPosition().x && posMouse.x < Bbox.getPosition().x+Bbox.getSize().x
                                                            && posMouse.y > Bbox.getPosition().y && posMouse.y < Bbox.getPosition().y+Bbox.getSize().y)
        return true;
    return false;
}
