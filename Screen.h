#ifndef Screen_h
#define Screen_h

#include "Window.h"
#include "TTY.h"

class Screen
{   
public:
    Screen();
    void Update();
    //void LateUpdate();
    void Draw();
    bool IsRunning();
    void ReadDataTTY();
private:
    Window window;
    sf::VertexArray graphLines;
    TTY sourceData;
    sf::Font font;
};


#endif