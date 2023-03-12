#include "Screen.h"
#include <thread>


Screen::Screen()
    : window("No name"), graphLines(sf::PrimitiveType::LineStrip,ADC_Data.getSizeBuffer()), gridOx(sf::PrimitiveType::Lines,(NUMBER_OF_LINES_HOR)),
        gridOy(sf::PrimitiveType::Lines,(NUMBER_OF_LINES_VERT))
{
    
    for (int i=0;i<NUMBER_OF_LINES_HOR/2;++i)
    {
        gridOx[2*i].color = sf::Color::Black;
        gridOx[2*i+1].color = sf::Color::Black;
        gridOx[2*i].position = sf::Vector2f(100,i==0?100:100+STEP*i);
        gridOx[2*i+1].position = sf::Vector2f(WINDOW_GRID_SIZE_HOR,i==0?100:100+STEP*i);
    }

    for (int i=0;i<NUMBER_OF_LINES_VERT/2;++i)
    {
        gridOy[2*i].color = sf::Color::Black;
        gridOy[2*i+1].color = sf::Color::Black;
        gridOy[2*i].position = sf::Vector2f(i==0?100:100+STEP*i,100);
        gridOy[2*i+1].position = sf::Vector2f(i==0?100:100+STEP*i,WINDOW_GRID_SIZE_VERT+100);
    }
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"))
            std::cout<<"Error reading font"<<std::endl;
}

void Screen::Update()
{   
    /*
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,50);
    uint16_t* ADC_buf = &ADC_Data.getData();
    for (int i=0;i<ADC_Data.getSizeBuffer();++i)
        {
            *ADC_buf++ = dist(gen);
            //*ADC_buf++ = i;
            //ADC_buf++;
        }
    */
   //while(1)
   //{
   // ADC_Data.readyReceiveData();
   // std::this_thread::sleep_for(std::chrono::milliseconds(10));
   //}

    //Screen::ReadDataTTY();
    ADC_Data.pseudo_data();
    Screen::convertADC_DataForScreen(&(ADC_Data.getData()));

    setStringOnDisplay(amplitude,font,"ampl: ",120,610,20);
    setStringOnDisplay(frequency,font,"freq: "+std::to_string(frequencyCalc(&(ADC_Data.getData())))+" Hz",250,610,20);
    setStringOnDisplay(period,font,"T: "+std::to_string(1/frequencyCalc(&(ADC_Data.getData()))*1000)+" ms",500,610,20);

    window.Update();
   // ADC_Data.readyReceiveData();
}

void Screen::Draw()
{
    window.BeginDraw();
    window.Draw(gridOx);
    window.Draw(gridOy);
    window.Draw(amplitude);
    window.Draw(frequency);
    window.Draw(period);
    window.Draw(graphLines);
    window.EndDraw();
}

bool Screen::IsRunning()
{
    while (window.IsOpen())
        return true;
    return false;
}

void Screen::ReadDataTTY()
{
    ADC_Data.readData();
}

void Screen::convertADC_DataForScreen(uint16_t* bufferADC)
{
    int offset = ADC_Data.offsetWithTrig();
    bufferADC += offset;
    int bufferSize = ADC_Data.getSizeBuffer();
    int coef = 5;
    for (int i=0;i<bufferSize-offset;++i)
    {
        graphLines[i].color = sf::Color::Blue;
        graphLines[i].position = sf::Vector2f(coef*i+100,SIZE_VERT-bufferADC[i]/COEF-100);
    }
}

float Screen::frequencyCalc(uint16_t* bufferADC)
{
    int startPoint =BUFFER_SIZE/10;
    int endpoint = BUFFER_SIZE - startPoint-1;
    int trigVal =ADC_Data.getTrigValue();
    float numCyclesADC = 1.5;
    bool flagRising1 = 0;
    int valFlagR1    = 0;
    int valFlagR2    = 0;
    for (int i=startPoint;i<endpoint;++i)
    {
        if (bufferADC[i] <trigVal && bufferADC[i+1] > trigVal && flagRising1)
        {
            valFlagR2 = i;
            return 1/((valFlagR2-valFlagR1)*0.000000928);
        }
        if (bufferADC[i] < trigVal && bufferADC[i+1] > trigVal)
        {
            flagRising1 =1;
            valFlagR1 = i;
        }
    }
    return 0;
}

void setStringOnDisplay(sf::Text& text, sf::Font& font, const std::string& str,
                         const float posX, const float posY, const unsigned int size,
                          sf::Color color)
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    sf::Vector2f position;
    position.x = posX;
    position.y = posY;
    text.setPosition(position);
}