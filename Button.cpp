
#include "Button.h"

Button::Button(const sf::Font& font,sf::Vector2f position, const std::string label)
    :Bbox(sf::Vector2f(BUTSIZEH,BUTSIZEV)),Bframe(sf::Vector2f(BUTSIZEH,BUTSIZEV)), Btext(label,font,20)
{
    Button::setPosition(position);
    Bbox.setFillColor(sf::Color(190,190,190));
    Btext.setFillColor(sf::Color::Black);

    Bframe.setOutlineThickness(3);
    Bframe.setOutlineColor(sf::Color(190,190,190));
}

bool Button::buttonIsPressed(const sf::RenderWindow& window)
{
    sf::Vector2i posMouse;
    posMouse = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && posMouse.x > Bbox.getPosition().x && posMouse.x < Bbox.getPosition().x+Bbox.getSize().x
                                                            && posMouse.y > Bbox.getPosition().y && posMouse.y < Bbox.getPosition().y+Bbox.getSize().y)
        {
            Bframe.setOutlineColor(sf::Color(50,205,50));
            return true;
        }
    Bframe.setOutlineColor(sf::Color(0x18,0x18,0x18));
    return false;
}
