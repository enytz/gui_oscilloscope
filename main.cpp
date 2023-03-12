#include <iostream>
#include "Screen.h"
#include <thread>

int main()
{
    Screen screen;

    while(screen.IsRunning())
    {
        screen.Update();
        screen.Draw();
    }
    return 0;
}