#include "TTY.h"
#include <thread>

TTY::TTY()
    :trigValue(TRIG_VALUE)
{
    for (int i=0;i<BUFFER_SIZE;++i)
    {
        buffer_ADC[i] = 0;
    }
    for (int i=0;i<BUFFER_SIZE*2;++i)
    {
        buffer[i] = 0;
    }

    serial_port = open("/dev/ttyUSB0",O_RDWR);
    //serial_port = open("/dev/ttyACM0",O_RDONLY);
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

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;  // prevent special interpretator of output bytes - output is raw data
    tty.c_oflag &= ~ONLCR;  // prevent conversion of newline to carriage return/line feed

    // setting time delay for waiting data
    tty.c_cc[VTIME] = 1;      // wait for up 1s (10*100ms), 100 ms =1
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
    tcflush(serial_port,TCIFLUSH);
    close(serial_port);
}

void TTY::readData()
{
    tcflush(serial_port,TCIFLUSH);
    int readBytes = 0;
    int numBytes = 0;
    int sizeOfBuffer = BUFFER_SIZE*2;

    uint8_t controlSequence[4]{0xde, 0xad, 0xba, 0xba};
    int counter =0;
    while (counter<4)
        {
            numBytes = read(serial_port,&buffer[counter],1);
            if (numBytes <0)
                {
                    perror("Error reading: ");
                    return;
                }
            if (buffer[counter] == controlSequence[counter])
                {
                    counter++;
                }
            else
                {
                    counter =0;
                }
        }
    do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            numBytes = read(serial_port,&buffer[readBytes],sizeOfBuffer-readBytes);
            std::cout<<numBytes<<' ';
            if (numBytes <0)
                {
                    perror("Error reading: ");
                    return;
                }
            else if (numBytes == 0)
                {
                    perror("No data ");
                }
            readBytes +=numBytes;
        }while(readBytes < sizeOfBuffer);
    std::cout<<'\n';
    for (int i=0;i<BUFFER_SIZE;++i)
        {
            buffer_ADC[i] = (uint16_t)(buffer[2*i] | (buffer[2*i+1]<<8));
        }
    std::cout<<"---------------------------------------------------\n";
}

void TTY::TransmitData(uint8_t symbol)
{
    int numWritingBytes = 0;
    int numBytes =0;
    do
    {
        numBytes = write(serial_port,&symbol,sizeof(symbol));
        numWritingBytes += numBytes;
        if (numWritingBytes < 0)
            {
                perror("Error transmit: ");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
    }while(numWritingBytes < 1);
    std::cout<<"Transfer is complited\n";
}

uint16_t TTY::offsetWithTrig()
{
    uint16_t preTrigValue = BUFFER_SIZE/4;
    bool flag =0;
    for (int i=preTrigValue;i<BUFFER_SIZE;++i)
    {
        if (buffer_ADC[i-1] < trigValue && buffer_ADC[i]> trigValue)
            flag =1;
        if (buffer_ADC[i]> trigValue && flag)
            return i-BUFFER_SIZE/4;
        
    }
    return 0;
}

void TTY::pseudo_data()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,360);
    float dt = BUFFER_SIZE/360.0;
    int Ampl = 1000;
    int f    = 10;
    int fi = M_PI*dist(gen)/180;
    for(int i=0;i<BUFFER_SIZE;++i)
    {
        buffer_ADC[i] = Ampl/2*sin(2*M_PI*dt*i*f/180+fi)+Ampl/2;      //sin
    }
}