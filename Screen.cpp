#include "Screen.h"
#include <thread>

#include <random>

Screen::Screen()
    : window("No name"), graphLines(sf::PrimitiveType::LineStrip,ADC_Data.getSizeBuffer())
{
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"))
            std::cout<<"Error reading font"<<std::endl;
}

void Screen::Update()
{   
    /*
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,1023);
    uint16_t* ADC_buf = &ADC_Data.getData();
    for (int i=0;i<ADC_Data.getSizeBuffer();++i)
        {
            *ADC_buf++ = dist(gen);
            //*ADC_buf++ = i;
            //ADC_buf++;
        }
    */
    Screen::ReadDataTTY();
    Screen::convertADC_DataForScreen(&ADC_Data.getData());
    window.Update();
}

void Screen::Draw()
{
    window.BeginDraw();
    window.Draw(graphLines);
    window.EndDraw();
}

bool Screen::IsRunning()
{
    while (window.IsOpen())
        return true;
    return false;
}

void Screen::ReadDataTTY()
{
    // add case not all reading data ( sizebuffer != num_bytes)
    ADC_Data.readData();
}

void Screen::convertADC_DataForScreen(uint16_t* bufferADC)
{
    int bufferSize = ADC_Data.getSizeBuffer();
    for (int i=0;i<bufferSize;++i)
    {
        graphLines[i].color = sf::Color::Blue;
        graphLines[i].position = sf::Vector2f(i+100,600-*bufferADC++/COEF-100);
    }
}