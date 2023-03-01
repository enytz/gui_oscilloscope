#include <iostream>
#include "Screen.h"
#include <thread>

#define MASSIZE 12
int main()
{
    Screen screen;
    
    while(screen.IsRunning())
    {
        screen.Update();
        screen.Draw();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}