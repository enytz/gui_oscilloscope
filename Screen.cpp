#include "Screen.h"
#include <thread>


Screen::Screen()
    : window("No name"), graphLines(sf::PrimitiveType::LineStrip),gridOx(sf::PrimitiveType::Lines,(NUMBER_OF_LINES_HOR)),
        gridOy(sf::PrimitiveType::Lines,(NUMBER_OF_LINES_VERT)),sweepButton(font),scaleButton(font), scaleCoef(2)
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

    sweepButton.setPosition(sf::Vector2f(1050,150));
    sweepButton.setLabel("mks/cell");
    scaleButton.setPosition(sf::Vector2f(1050,225));
    scaleButton.setLabel("scale X");
}

void Screen::Update()
{   
    Screen::ReadDataTTY();
    //ADC_Data.pseudo_data();
    Screen::convertADC_DataForScreen(&(ADC_Data.getData()));

    setStringOnDisplay(frequency,font,"Freq: "+std::to_string(frequencyCalc(&(ADC_Data.getData()))/1000)+" kHz",120,610,20,sf::Color::White);
    setStringOnDisplay(period,font,"T: "+std::to_string(1/frequencyCalc(&(ADC_Data.getData()))*1000)+" ms",450,610,20,sf::Color::White);
    setStringOnDisplay(mksPerCell,font,"mks/cell "+std::to_string(50*ADC_Data.timeConversion()*1000000/scaleCoef), 700,610,20,sf::Color::White);

    window.Update();
    ADC_Data.TransmitData('r');
}

void Screen::LateUpdate()
{
    static int counterSweep = 0;
    static int counterScale = 0;
    bool flag   = 0;
    if (sweepButton.buttonIsPressed(window.GetRef()))
    {
        counterSweep++;
        flag = 1;
    }
    else if(scaleButton.buttonIsPressed(window.GetRef()))
    {
        counterScale++;
        flag = 1;

    }
    if ((counterSweep || counterScale) && !flag)
        {
            counterSweep?CBSweep():CBScale();
        }

    if (!flag)
    {
        counterSweep = 0; 
        counterScale = 0;
    }   
}

void Screen::Draw()
{
    window.BeginDraw();
    window.Draw(gridOx);
    window.Draw(gridOy);

    window.Draw(frequency);
    window.Draw(period);
    window.Draw(graphLines);
    window.Draw(mksPerCell);

    window.Draw(sweepButton.getRefBframe());
    window.Draw(sweepButton.getRefBbox());
    window.Draw(sweepButton.getRefBtext());

    window.Draw(scaleButton.getRefBframe());
    window.Draw(scaleButton.getRefBbox());
    window.Draw(scaleButton.getRefBtext());
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
    int bufferSize = ADC_Data.getSizeBuffer()-offset;
    graphLines.resize(bufferSize);

    for (int i=0;i<bufferSize;++i)
    {
        graphLines[i].color = (scaleCoef*i)<WINDOW_GRID_SIZE_HOR-100? sf::Color::Blue : sf::Color(0x18,0x18,0x18);      // in case if line > cell grid 0X
        graphLines[i].position = sf::Vector2f(scaleCoef*i+100,SIZE_VERT-bufferADC[i]/COEF-100);
    }
}

float Screen::frequencyCalc(uint16_t* bufferADC)
{
    static float frequencyValue = 0;
    static int countMeas        = 0;
    int startPoint      = BUFFER_SIZE/10;
    int endpoint        = BUFFER_SIZE - startPoint-1;
    int trigVal         = ADC_Data.getTrigValue();
    bool flagRising1    = 0;
    int valFlagR1       = 0;
    int valFlagR2       = 0;
    for (int i=startPoint;i<endpoint;++i)
    {
        if (bufferADC[i] <trigVal && bufferADC[i+1] > trigVal && flagRising1)
        {
            valFlagR2 = i;
            countMeas++;
            if (countMeas >= 10)
            {
                frequencyValue = 1/((valFlagR2-valFlagR1)*ADC_Data.timeConversion());
                return frequencyValue;
            }
            return frequencyValue;
            //return 1/((valFlagR2-valFlagR1)*timeConversion);
        }
        if (bufferADC[i] < trigVal && bufferADC[i+1] > trigVal)
        {
            flagRising1 =1;
            valFlagR1 = i;
        }
    }
    //return 0;
    return frequencyValue;
}

void Screen::CBScale()
{
    static int numPushButton=0;
    numPushButton++;
    switch (numPushButton)
    {
        case 1:
            setCoef(5);
            break;
        case 2:
            setCoef(10);
            break;
        default:
            setCoef(2);
            numPushButton =0;
            break;
    }
    std::cout<<"CBScale\n";
}

void Screen::CBSweep()
{
    ADC_Data.TransmitData('c');
    ADC_Data.setFlagIncrementCyclesADC();
    std::cout<<"CBSweep\n";
    
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