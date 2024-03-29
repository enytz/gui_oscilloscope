
#include "TTY.h"


#if defined (__linux) || defined (unix)
    TTY::TTY()
        :numOfCycles{ 1.5, 7.5, 13.5, 28.5, 41.5, 55.5, 71.5, 239.5 }, flagIncrementCycles(0), trigValue(TRIG_VALUE)
    {
        for (int i = 0;i < BUFFER_SIZE;++i)
        {
            buffer_ADC[i] = 0;
        }
        for (int i = 0;i < BUFFER_SIZE * 2;++i)
        {
            buffer[i] = 0;
        }

        serial_port = open("/dev/ttyUSB0", O_RDWR);
        //serial_port = open("/dev/ttyACM0",O_RDONLY);
        // https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
        if (tcgetattr(serial_port, &tty) != 0)
        {
            perror("Error opening serial port: ");
            return;
        }
        if (tcgetattr(serial_port, &oldtty) != 0)
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
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

        tty.c_oflag &= ~OPOST;  // prevent special interpretator of output bytes - output is raw data
        tty.c_oflag &= ~ONLCR;  // prevent conversion of newline to carriage return/line feed

        // setting time delay for waiting data
        tty.c_cc[VTIME] = 1;      // wait for up 1s (10*100ms), 100 ms =1
        tty.c_cc[VMIN] = sizeof(BUFFER_SIZE * sizeof(buffer_ADC[0]));

        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);

        if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
        {
            perror("Error from tcsetattr: ");
            return;
        }
    }

    TTY::~TTY()
    {
        tcsetattr(serial_port, TCSANOW, &oldtty);
        tcflush(serial_port, TCIFLUSH);
        close(serial_port);
    }

    void TTY::readData()
    {
        tcflush(serial_port, TCIFLUSH);
        int readBytes = 0;
        int numBytes = 0;
        int sizeOfBuffer = BUFFER_SIZE * 2;

        uint8_t controlSequence[4]{ 0xde, 0xad, 0xba, 0xba };
        int counter = 0;
        while (counter < 4)
        {
            numBytes = read(serial_port, &buffer[counter], 1);
            if (numBytes < 0)
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
                counter = 0;
            }
        }
        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            numBytes = read(serial_port, &buffer[readBytes], sizeOfBuffer - readBytes);
            std::cout << numBytes << ' ';
            if (numBytes < 0)
            {
                perror("Error reading: ");
                return;
            }
            else if (numBytes == 0)
            {
                perror("No data ");
            }
            readBytes += numBytes;
        } while (readBytes < sizeOfBuffer);
        std::cout << '\n';
        for (int i = 0;i < BUFFER_SIZE;++i)
        {
            buffer_ADC[i] = (uint16_t)(buffer[2 * i] | (buffer[2 * i + 1] << 8));
        }
        std::cout << "---------------------------------------------------\n";
    }

    void TTY::TransmitData(uint8_t symbol)
    {
        int numBytes = 0;
        do
        {
            numBytes = write(serial_port, &symbol, sizeof(symbol));
            if (numBytes < 0)
            {
                perror("Error transmit: ");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        } while (numBytes < 1);
        std::cout << "Transfer is complited\n";
    }

#elif defined (_WIN32)

    TTY::TTY()
	    :numOfCycles{ 1.5, 7.5, 13.5, 28.5, 41.5, 55.5, 71.5, 239.5 }, flagIncrementCycles(0), trigValue(TRIG_VALUE), hserial(NULL), comPortName("\\\\.\\COM10")
    {
        for (int i = 0;i < BUFFER_SIZE;++i)
        {
            buffer_ADC[i] = 0;
        }
        for (int i = 0;i < BUFFER_SIZE * 2;++i)
        {
            buffer[i] = 0;
        }

	    hserial = CreateFile(comPortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);   // compile g++
        //hserial = CreateFile(L"\\\\.\\COM10", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // for compile VS

	    if (hserial == INVALID_HANDLE_VALUE)
	    {
		    std::cout << "Error opening port\n";
	    }
        else
        {
            std::cout << "Serial port is open\n";
        }
	    //COMMTIMEOUTS htimeSettings;
        htimeSettings.ReadIntervalTimeout = MAXDWORD;		//ms
	    htimeSettings.ReadTotalTimeoutConstant = 10000;
	    htimeSettings.ReadTotalTimeoutMultiplier = MAXDWORD;
	    htimeSettings.WriteTotalTimeoutMultiplier = 0;
	    htimeSettings.WriteTotalTimeoutConstant = 500;

	    //DCB comSetting;
	    //memset(&comSetting, 0, sizeof(comSetting));
        GetCommState(hserial, &comSetting);
	    comSetting.DCBlength = sizeof(DCB);
	    comSetting.BaudRate = CBR_115200;
	    comSetting.ByteSize = 8;
	    comSetting.fParity = NOPARITY;
	    comSetting.StopBits = ONESTOPBIT;
	    comSetting.fBinary = TRUE;
	    comSetting.fParity = FALSE;
	    comSetting.fOutxCtsFlow = FALSE;
	    comSetting.fOutxDsrFlow = FALSE;
	    comSetting.fDtrControl = DTR_CONTROL_DISABLE;
	    comSetting.fRtsControl = RTS_CONTROL_DISABLE;
	    comSetting.fDsrSensitivity = FALSE;
	    comSetting.fTXContinueOnXoff = FALSE;

	    SetCommState(hserial, &comSetting);
	    bool state = GetCommState(hserial, &comSetting);
	    if (!state)
	    {
		    std::cout << "Error set settings DCB structure\n";
	    }

        GetCommTimeouts(hserial, &htimeSettings);
	    SetCommTimeouts(hserial, &htimeSettings);
	    PurgeComm(hserial, PURGE_RXCLEAR | PURGE_TXCLEAR);
    }

    TTY::~TTY()
    {
	    PurgeComm(hserial, PURGE_RXCLEAR | PURGE_TXCLEAR);
	    CloseHandle(hserial);
    }

    void TTY::readData()
    {
        DWORD readBytes = 0;
        DWORD numBytes = 0;
        DWORD sizeOfBuffer = BUFFER_SIZE * 2;
        uint8_t controlSequence[4]{ 0xde, 0xad, 0xba, 0xba };
        int counter = 0;
        while (counter < 4)
        {
            if (!ReadFile(hserial, buffer, (DWORD)1, &numBytes, NULL))
            {
                std::cout << "Error control sequence " << GetLastError() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //return;
            }
            if (buffer[0] == controlSequence[counter])
            {
                counter++;
            }
            else
            {
                counter = 0;
            }
        }
        numBytes = 0;
        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            if (!ReadFile(hserial, &buffer[readBytes], sizeOfBuffer-readBytes, &numBytes, NULL))
            {
                std::cout << "Error reading " << GetLastError() << std::endl;
                return;
            }
            else if (numBytes == 0)
            {
                std::cout << "No data: " << GetLastError() << std::endl;
            }
            std::cout << numBytes << ' ';
            readBytes += numBytes;
        } while (readBytes < sizeOfBuffer);
        std::cout << '\n';
        for (int i = 0;i < BUFFER_SIZE;++i)
        {
            buffer_ADC[i] = (uint16_t)(buffer[2 * i+1]<<8 | (buffer[2 * i]));
        }
        std::cout << "---------------------------------------------------\n";
    }

    void TTY::TransmitData(const uint8_t symbol)
    {
        DWORD numBytes = 0;
        do
        {
            if (!WriteFile(hserial, &symbol, DWORD(sizeof(symbol)), &numBytes, NULL))
            {
                std::cout << "Error " << GetLastError() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        } while (numBytes < 1);
        std::cout << "Transfer is complited\n";
    }
#endif

uint16_t TTY::offsetWithTrig()
{
    uint16_t preTrigValue = BUFFER_SIZE / 10;
    bool flag = 0;
    for (int i = preTrigValue;i < BUFFER_SIZE;++i)
    {
        if (buffer_ADC[i - 1] < trigValue && buffer_ADC[i]> trigValue)
            flag = 1;
        if (buffer_ADC[i] > trigValue && flag)
            return i - BUFFER_SIZE / 10;
    }
    return 0;
}

float TTY::timeConversion()
{
    static int idx = 0;
    if (flagIncrementCycles)
    {
        idx++;
        if (idx > 7)
            idx = 0;
        flagIncrementCycles = 0;
    }
    return 0.000000821 + numOfCycles[idx] * (0.000000071);  // 821ns - time sampling 10B ADC,   1/14MHz = 71 ns
}

void TTY::pseudo_data()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 360);
    float dt = BUFFER_SIZE / 360.0;
    int Ampl = 1000;
    float f = 2;
    float fi = M_PI * dist(gen) / 180.0;
    for (int i = 0;i < BUFFER_SIZE;++i)
    {
       buffer_ADC[i] = Ampl / 2 * sin(2 * M_PI * dt * i * f / 180 + fi) + Ampl / 2;      //sin
    }
}