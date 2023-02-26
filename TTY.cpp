#include "TTY.h"
#include <thread>
TTY::TTY()
{
    for (int i=0;i<BUFFER_SIZE;++i)
    {
        buffer_ADC[i] = 0;
    }

    serial_port = open("/dev/ttyUSB0",O_RDONLY);
    // https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    if (tcgetattr(serial_port,&tty) !=0)
    {
        perror("Error opening serial port: ");
        return;
    }
    if (tcgetattr(serial_port,&oldtty) != 0)
    {
        perror("Error reading settings: ");
    }


    tty.c_cflag &= ~PARENB; // clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB; // clear stop field, 1 stop bit
    tty.c_cflag &= ~CSIZE;  // clear all bits for set data size
    tty.c_cflag |= CS8;     // set data size  = 8 bit
    tty.c_cflag &= ~CRTSCTS;    // disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;  // turn on read& ignore ctrl lines

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;   // disable echo
    tty.c_lflag &= ~ECHOE;  // disable earsure
    tty.c_lflag &= ~ECHONL; // disable new-line echo
    tty.c_lflag &= ~ISIG;   // disable interpretation of INTR, QUIT and SUSP

    //tty.c_iflag &= ~(IXON | IXANY); //turn off s/w flow ctrl
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;  // prevent special interpretator of output bytes
    tty.c_oflag &= ~ONLCR;  // prevent conversion of newline to carriage return/line feed

    // setting time delay for waiting data
    tty.c_cc[VTIME] = 10;      // wait for up 1s (10*100ms), 100 ms =1
    tty.c_cc[VMIN] = sizeof(BUFFER_SIZE*sizeof(buffer_ADC[0]));

    cfsetispeed(&tty,B115200);
    cfsetospeed(&tty,B115200);

    if (tcsetattr(serial_port,TCSANOW,&tty) !=0)
    {
        perror("Error from tcsetattr: ");
        return;
    }
}

TTY::~TTY()
{
    tcsetattr(serial_port,TCSANOW, &oldtty);
    close(serial_port);
    //std::cout<<"~tty";
    //std::cout.flush();
}

void TTY::readData()
{
        tcflush(serial_port,TCIFLUSH);
        int num_bytes = read(serial_port,&buffer_ADC,sizeof(buffer_ADC));
        //int readBytes = 0;
        //int numBytes = 0;

        //do
        //{
         //   numBytes = read(serial_port,&buffer_ADC[readBytes],sizeof(buffer_ADC)-readBytes);
         //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (num_bytes <0)
                {
                    perror("Error reading: ");
                    return;
                }
            else if (num_bytes == 0)
                {
                    perror("Not full reading buffer: ");
                    return;
                }
        //    readBytes +=numBytes;
       // }while(readBytes< BUFFER_SIZE*sizeof(buffer_ADC[0]) );
}