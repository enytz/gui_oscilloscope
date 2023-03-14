#include "Screen.h"
#include <thread>


Screen::Screen()
    : window("No name"), graphLines(sf::PrimitiveType::LineStrip),gridOx(sf::PrimitiveType::Lines,(NUMBER_OF_LINES_HOR)),
        gridOy(sf::PrimitiveType::Lines,(NUMBER_OF_LINES_VERT)),but(font)
{
    for (int i=0;i<NUMBER_OF_LINES_HOR/2;++i)
    {
        gridOx[2*i].color = sf::Color(128,128,128);
        gridOx[2*i+1].color = sf::Color(128,128,128);
        gridOx[2*i].position = sf::Vector2f(100,i==0?100:100+STEPVERT*i);
        gridOx[2*i+1].position = sf::Vector2f(WINDOW_GRID_SIZE_HOR,i==0?100:100+STEPVERT*i);
    }

    for (int i=0;i<NUMBER_OF_LINES_VERT/2-1;++i)
    {
        gridOy[2*i].color = sf::Color(128,128,128);
        gridOy[2*i+1].color = sf::Color(128,128,128);
        gridOy[2*i].position = sf::Vector2f(i==0?100:100+STEPHOR*i,100);
        gridOy[2*i+1].position = sf::Vector2f(i==0?100:100+STEPHOR*i,WINDOW_GRID_SIZE_VERT+100);
    }
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf"))
            std::cout<<"Error reading font"<<std::endl;
}

void Screen::Update()
{   
    //Screen::ReadDataTTY();
    ADC_Data.pseudo_data();
    Screen::convertADC_DataForScreen(&(ADC_Data.getData()));

    setStringOnDisplay(frequency,font,"Freq: "+std::to_string(frequencyCalc(&(ADC_Data.getData()))/1000)+" kHz",120,610,20,sf::Color::White);
    setStringOnDisplay(period,font,"T: "+std::to_string(1/frequencyCalc(&(ADC_Data.getData()))*1000)+" ms",450,610,20,sf::Color::White);

    window.Update();
   // ADC_Data.readyReceiveData();
}

void Screen::LateUpdate()
{
    
    static int counter = 0;
    bool flag   = 0;
    if (but.buttonIsPressed(window.GetRef()))
    {
        counter++;
        flag =1;
        std::cout<<counter<<'\n';
    }
    if (!flag)
        counter = 0;
    flag =0;
    
}

void Screen::Draw()
{
    window.BeginDraw();
    window.Draw(gridOx);
    window.Draw(gridOy);

    window.Draw(but.getRefBbox());
    window.Draw(but.getRefBtext());

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

    graphLines.resize(bufferSize-offset);

    int coef = 5;
    for (int i=0;i<bufferSize-offset;++i)
    {
        graphLines[i].color = sf::Color::Blue;
        graphLines[i].position = sf::Vector2f(coef*i+100,SIZE_VERT-bufferADC[i]/COEF-100);
    }
}

float Screen::frequencyCalc(uint16_t* bufferADC)
{
    int startPoint      = BUFFER_SIZE/10;
    int endpoint        = BUFFER_SIZE - startPoint-1;
    int trigVal         = ADC_Data.getTrigValue();
    float numCyclesADC  = 1.5;
    float timeSampl     = 0.000000821;
    float timeConversion= timeSampl+numCyclesADC*(1.0/14000000);
    bool flagRising1    = 0;
    int valFlagR1       = 0;
    int valFlagR2       = 0;
    for (int i=startPoint;i<endpoint;++i)
    {
        if (bufferADC[i] <trigVal && bufferADC[i+1] > trigVal && flagRising1)
        {
            valFlagR2 = i;
            return 1/((valFlagR2-valFlagR1)*timeConversion);
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