#include "Screen.h"
#include <thread>

#include <random>

Screen::Screen()
    : window("No name"), graphLines(sf::PrimitiveType::LineStrip,sourceData.getSizeBuffer())
{
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"))
            std::cout<<"Error reading font"<<std::endl;
}

void Screen::Update()
{
    window.Update();
}

void Screen::Draw()
{
    window.BeginDraw();
    int mas[sourceData.getSizeBuffer()];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,400);
    for (int i=0;i<sourceData.getSizeBuffer();++i)
        {
            mas[i] = dist(gen);
            graphLines[i].color = sf::Color::Blue;
            graphLines[i].position = sf::Vector2f(i*5,500-mas[i]);
        }

    window.Draw(graphLines);
    window.EndDraw();
}

bool Screen::IsRunning()
{
    while (window.IsOpen())
        return true;
}

void Screen::ReadDataTTY()
{
    // add case not all reading data ( sizebuffer != num_bytes)
    sourceData.readData();
}