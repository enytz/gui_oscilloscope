#ifndef Button_h
#define Button_h

#include <SFML/Graphics.hpp>

class Button
{
    public:
        Button(const sf::Font& font);
        Button(){}
        bool buttonIsPressed(const sf::RenderWindow& window);
        void setSize(sf::Vector2f size){Bbox.setSize(size);}
        void setPosition(sf::Vector2f position){Bbox.setPosition(position); Btext.setPosition(sf::Vector2f(position.x+40,position.y+40));}
        void setLabel(const std::string& str){Btext.setString(str);}
        const sf::RectangleShape& getRefBbox() const {return Bbox;}
        const sf::Text& getRefBtext()const {return Btext;}
    private:
    sf::RectangleShape Bbox;
    sf::Text Btext;
};



#endif