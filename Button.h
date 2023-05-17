#ifndef Button_h
#define Button_h

#include <SFML/Graphics.hpp>

#define BUTSIZEH 100
#define BUTSIZEV 50
class Button
{
public:
    Button(const sf::Font& font, sf::Vector2f position = sf::Vector2f(200, 200), const std::string label = "label1");
    Button() {}
    bool buttonIsPressed(const sf::RenderWindow& window);
    void setSize(sf::Vector2f size) { Bbox.setSize(size); Bframe.setSize(size);}
    void setPosition(sf::Vector2f position) { Bbox.setPosition(position);Bframe.setPosition(position); Btext.setPosition(sf::Vector2f(position.x + 10, position.y + 10)); }
    void setLabel(const std::string& str) { Btext.setString(str); }
    const sf::RectangleShape& getRefBbox() const { return Bbox; }
    const sf::RectangleShape& getRefBframe() const { return Bframe; }
    const sf::Text& getRefBtext()const { return Btext; }
private:
    sf::RectangleShape Bbox;
    sf::RectangleShape Bframe;
    sf::Text Btext;
};



#endif