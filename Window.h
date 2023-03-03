#ifndef WINDOW_h
#define WINDOW_h

#include <SFML/Graphics.hpp>
#include <string>

#define SIZE_HOR 1200
#define SIZE_VERT 700
class Window
{
    public:
        Window(const std::string& windowName);
        void Update();
        void BeginDraw();
        void Draw(const sf::Drawable& drawable);
        void EndDraw();
        bool IsOpen() const;
    private:
        sf::RenderWindow window;
};
#endif