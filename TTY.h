#ifndef TTY_h
#define TTY_h

#define BUFFER_SIZE 450
#define TRIG_VALUE  900     // preload Value

#include <iostream>
#include <thread>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

#if defined (__linux) || defined (unix)
    #include <fcntl.h>      // Contains file control like O_RDWR
    #include <errno.h>      // Error integer
    #include <termios.h>    // Contains POSIX terminal control definitions
    #include <unistd.h>     // write(), read(), close()

#elif defined (_WIN32)
    #include <Windows.h>
#endif


struct TTY
{
public:
    TTY();
    ~TTY();
    void readData();
    void TransmitData(const uint8_t symbol);
    uint16_t& getData() { return *buffer_ADC; }
    int getSizeBuffer() const { return BUFFER_SIZE; }
    int getTrigValue() const { return trigValue; }
    void setTrigValue(int val) { trigValue = val; }
    uint16_t offsetWithTrig();
    float timeConversion();
    void setFlagIncrementCyclesADC() { flagIncrementCycles = 1; }
    void pseudo_data();
private:

    uint16_t buffer_ADC[BUFFER_SIZE];
    uint8_t buffer[BUFFER_SIZE * 2];

    float numOfCycles[8];
    bool flagIncrementCycles;

    int trigValue;
    #if defined (__linux) || defined (unix)
        int serial_port;
        termios tty, oldtty;
    #elif defined (_WIN32)
        HANDLE hserial;
        DCB comSetting;
        COMMTIMEOUTS htimeSettings;
        std::string comPortName;
    #endif
};
#endif 


