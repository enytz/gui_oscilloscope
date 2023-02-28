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
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    /*
    TTY tty;
    tty.readData();
    int bufSize = tty.getSizeBuffer();
    uint16_t *ADC_buf = &tty.getData();

    for (int i=1;i<bufSize+1;++i)
    {
        std::cout<<'\t'<<(int)*ADC_buf++<<' ';
        if (!(i%10))
            std::cout<<std::endl;

    }
    std::cout<<'\n';
    */
    return 0;
}