#ifndef TTY_h
#define TTY_h

#include <fcntl.h>      // Contains file control like O_RDWR
#include <errno.h>      // Error integer
#include <termios.h>    // Contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

//#include <memory>
//#include <atomic>
//#include <thread>
#define BUFFER_SIZE 250

#include <iostream>

struct TTY
{
    public:
        TTY();
        ~TTY();
        void readData();
        uint16_t& getData() {return *buffer_ADC;}
        int getSizeBuffer() const {return BUFFER_SIZE;}
        int getReadBytes() const {return readBytes;}
    private:
        uint16_t buffer_ADC[BUFFER_SIZE];
        uint8_t buffer[BUFFER_SIZE*2];

        int readBytes;
        int serial_port;
        termios tty,oldtty;
};



#endif