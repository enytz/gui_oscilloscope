#ifndef TTY_h
#define TTY_h

#include <fcntl.h>      // Contains file control like O_RDWR
#include <errno.h>      // Error integer
#include <termios.h>    // Contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

#include <math.h>       // generate pseudo data
#include <random>

#define BUFFER_SIZE 200
#define TRIG_VALUE  500

#include <iostream>

struct TTY
{
    public:
        TTY();
        ~TTY();
        void readData();
        void readyReceiveData();
        uint16_t& getData() {return *buffer_ADC;}
        int getSizeBuffer() const {return BUFFER_SIZE;}
        int getTrigValue() const {return trigValue;}
        void setTrigValue(int val) {trigValue = val;}
        uint16_t offsetWithTrig();

        void pseudo_data();
    private:

        uint16_t buffer_ADC[BUFFER_SIZE];
        uint8_t buffer[BUFFER_SIZE*2];

        int trigValue;
        int serial_port;
        termios tty,oldtty;
};

#endif