#ifndef Screen_h
#define Screen_h

#include "Window.h"
#include "TTY.h"

//#define ADC12B
#define ADC10B
//#define ADC8B

#ifdef ADC12B 
    #define MAX_SCALE_VALUE 4095
    #define COEF            10.2375
#endif

#ifdef ADC10B 
    #define MAX_SCALE_VALUE 1023
    #define COEF            2.5575
#endif

#ifdef ADC8B 
    #define MAX_SCALE_VALUE 255
    #define COEF            0.6375
#endif

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
    sf::RectangleShape oXoY;
    TTY ADC_Data;
    void convertADC_DataForScreen(uint16_t* bufferADC);
    sf::Font font;
};


#endif