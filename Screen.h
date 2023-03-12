#ifndef Screen_h
#define Screen_h

#include "Window.h"
#include "TTY.h"
#include <string>


#define STEP                    50
#define WINDOW_GRID_SIZE_VERT   500
#define WINDOW_GRID_SIZE_HOR    1000
#define NUMBER_OF_LINES_VERT    (WINDOW_GRID_SIZE_HOR/STEP*2)
#define NUMBER_OF_LINES_HOR     ((WINDOW_GRID_SIZE_VERT/STEP+1)*2)

//#define ADC12B
#define ADC10B
//#define ADC8B

#ifdef ADC12B 
    #define MAX_SCALE_VALUE 4095
    #define COEF            8.19
#endif

#ifdef ADC10B 
    #define MAX_SCALE_VALUE 1023
    #define COEF            2.046
#endif

#ifdef ADC8B 
    #define MAX_SCALE_VALUE 255
    #define COEF            0.51
#endif

void setStringOnDisplay(sf::Text& text, sf::Font& font, const std::string& str,
                         const float posX, const float posY, const unsigned int size = 30,
                          sf::Color color = sf::Color::Black);

class Screen
{   
public:
    Screen();
    void Update();
    void Draw();
    bool IsRunning();
    void ReadDataTTY();
private:
    Window window;
    sf::VertexArray graphLines;
    sf::VertexArray gridOx;
    sf::VertexArray gridOy;
    TTY ADC_Data;
    void convertADC_DataForScreen(uint16_t* bufferADC);
    float frequencyCalc(uint16_t* bufferADC);
    sf::Font font;
    sf::Text frequency;
    sf::Text amplitude;
    sf::Text period;
};


#endif