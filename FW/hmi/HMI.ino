#define HMI_STARTING_TIME 3000//0,5sec
//#define HMI_STARTUP_TIME 5000 //5sec
#define HMI_IDLE_TIME 50 //0,05sec to be in sync with Acoustics
#define HMI_TX_PIN 2
#define HMI_RX_PIN 3
#include "ecu_extract.h"
#include "a_IF1.h"
#include <Wire.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define LED_PIN 3

// Color definitions
#define ST77XX_BLACK       0x0000  ///<   0,   0,   0
#define ST77XX_GREEN       0x07E0  ///<   0, 255,   0

// Parameters for Blinker Size
#define a 6
#define b a+g
#define c 12
#define d 14
#define e a
#define f 18
#define g 6

#define PDU_TIMEOUT 500

// Color definitions
#define _BLACK       0x0000  ///<   0,   0,   0
#define _NAVY        0x000F  ///<   0,   0, 123
#define _DARKGREEN   0x03E0  ///<   0, 125,   0
#define _DARKCYAN    0x03EF  ///<   0, 125, 123
#define _MAROON      0x7800  ///< 123,   0,   0
#define _PURPLE      0x780F  ///< 123,   0, 123
#define _OLIVE       0x7BE0  ///< 123, 125,   0
#define _LIGHTGREY   0xC618  ///< 198, 195, 198
#define _DARKGREY    0x7BEF  ///< 123, 125, 123
#define _BLUE        0x001F  ///<   0,   0, 255
#define _GREEN       0x07E0  ///<   0, 255,   0
#define _CYAN        0x07FF  ///<   0, 255, 255
#define _RED         0xF800  ///< 255,   0,   0
#define _MAGENTA     0xF81F  ///< 255,   0, 255
#define _YELLOW      0xFFE0  ///< 255, 255,   0
#define _WHITE       0xFFFF  ///< 255, 255, 255
#define _ORANGE      0xFD20  ///< 255, 165,   0
#define _GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define _PINK        0xFC18  ///< 255, 130, 198
#define _GREY        0x2104  // Dark grey 16 bit colour
#define USE_LIGHT_SCHEME
#define BG_COLOR _BLACK
#define TXT_COLOR _WHITE
#define BG_GREY _LIGHTGREY
#define IPC_ADDRESS 8
#define height 128
#define width 160
#define fontHeight 8
#define fontWidth 5
 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(HMI_RX_PIN,HMI_TX_PIN); // pin 2 = TX, pin 3 = RX (unused)

static const char _sclk = 13;
static const char  _mosi = 11;
static const char _cs = 10;
static const char _dc = 9;
static const char _rst = 8;

Adafruit_ST7735 tft = Adafruit_ST7735(_cs, _dc, _rst);

enum hmi_state {HMI_INIT, HMI_STARTUP, HMI_STARTED, HMI_IDLE, HMI_REFRESH} hmi_stateVariable;
uint32_t hmi_msCounts=0,war_msCounts;
uint32_t hmi_prevMillis=0;
int i=0;
String currentODOString = "----";
uint8_t ODOStringLength = 4;
int bargraph=0;
char *units = "km/h";
uint8_t lastUnit = 2;
String printTemp = "";
#define initialTemp -1000
#define initialTempValue "  --.-"
String unit = "C";
int tempMantis = 0;
uint8_t tempExp = 0; 
int pl = 80,q = 94,h = 20;
bool available_warning=false;
void hmi_state_machine() {
 if(hmi_prevMillis!=millis()) {
  hmi_msCounts+=millis()-hmi_prevMillis;
  hmi_prevMillis=millis();
 }
uint16_t prevSpeed=511;
switch (hmi_stateVariable){
  
  case HMI_INIT:
     hmi_stateVariable = HMI_STARTUP;
     hmi_msCounts=0;
     tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
      tft.setRotation(1);
      tft.fillScreen(ST77XX_BLACK);
     pinMode(LED_PIN, OUTPUT);
      digitalWrite(LED_PIN, HIGH);
  
      
  break;

  case HMI_STARTUP:
    tft.setTextColor( _ORANGE);
    tft.setTextSize(2);
    tft.setCursor(25, 32);
    tft.print("Powered");
    
    tft.setCursor(42, 57);
    tft.print("by");
    delay(HMI_STARTING_TIME/4);
    tft.setCursor(50, 82);
    tft.print("VISTEON");
    
    if(hmi_msCounts >= HMI_STARTING_TIME)
    {
        tft.fillScreen(ST77XX_BLACK);
        hmi_stateVariable = HMI_STARTED;
        hmi_msCounts = 0;
    }
   
   case HMI_STARTED:
    if(hmi_msCounts >= HMI_STARTING_TIME)
    {
      hmi_stateVariable = HMI_IDLE;
      hmi_msCounts = 0;
    }  
     break; 

  case HMI_IDLE:
    if(hmi_msCounts >= HMI_IDLE_TIME)
    {
      hmi_stateVariable = HMI_REFRESH;
    }
     break; 
     

    case HMI_REFRESH:
      bool checkPDU = pduStatus();
      if (checkPDU){
        if(available_warning){
          available_warning=false;
          drawWarning(available_warning);
        }
        
        uint32_t Odometer = getOdometer();
        uint16_t Speed = getSpeed();
        uint16_t MaxSpeed = getMaxSpeed();
        uint8_t SpeedUnit = getSpeedUnit();
        uint8_t BlinkerLeft = getBlinkerLeft();
        uint8_t BlinkerRight = getBlinkerRight();
        int Temp = getTemperature();
        uint8_t TempUnit =  getTempUnit();
  
        drawBlinkers(BlinkerLeft, BlinkerRight);
        if(prevSpeed!=Speed){
          prevSpeed=Speed;
          drawSpeedo(Speed,SpeedUnit,MaxSpeed);
        }
        
        drawODO(Odometer, SpeedUnit);
        drawTemp(Temp, TempUnit);
        
        war_msCounts=0;
      }
      else{
         Serial.println(war_msCounts);
        war_msCounts +=hmi_msCounts;
        if(war_msCounts>PDU_TIMEOUT&&!available_warning){
            available_warning=true;
            drawWarning(available_warning);
        }
      }
        hmi_stateVariable = HMI_IDLE;
        hmi_msCounts = 0;
      

      break;
    }
  }

void drawBlinkers(int BlinkerLeft, int BlinkerRight){

   if(BlinkerLeft == 1) {

     tft.fillRect(f, b, d, c, ST77XX_GREEN);
     tft.fillTriangle( f,a,e,b+c/2,f,b+c+g,ST77XX_GREEN);
    }
    else {
     tft.fillRect(f, b, d, c, ST77XX_BLACK);
     tft.fillTriangle( f,a,e,b+c/2,f,b+c+g,ST77XX_BLACK);
   }

   if(BlinkerRight == 1) {
     tft.fillRect(160-f-d, b, d, c, ST77XX_GREEN);
     tft.fillTriangle( width-f,a,width-e,b+c/2,width-f,b+c+g,ST77XX_GREEN);
    }
    else {
     tft.fillRect(160-f-d, b, d, c, ST77XX_BLACK);
     tft.fillTriangle( width-f,a,width-e,b+c/2,width-f,b+c+g,ST77XX_BLACK);
   }
  
}

void drawODO(uint32_t ODO, uint8_t SpeedUnit){

  if (SpeedUnit == 0){
    ODO = ODO * 0.6213;
  }
  if(ODO>99999&&SpeedUnit){
    currentODOString="+99999";
  }
  else if(ODO>62129&&!SpeedUnit){
    currentODOString="+62129";
  } 
  else{
    currentODOString = "     "+(String)ODO+" ";
  }
  ODOStringLength = currentODOString.length()+1;
  tft.setCursor(width - fontWidth*(ODOStringLength+3)-6 , height - fontHeight);
  tft.setTextColor(TXT_COLOR, BG_COLOR);
  tft.setTextSize(1);
  tft.print(currentODOString);

  

  if (SpeedUnit == lastUnit){}
  else if (SpeedUnit == 1){
    tft.setCursor(width - 3*fontWidth , height - fontHeight);
    tft.setTextColor(TXT_COLOR, BG_COLOR);
    tft.setTextSize(1);
    tft.print("km");
    lastUnit = 1;
  }else{
    tft.setCursor(width - 3*fontWidth , height - fontHeight);
    tft.setTextColor(TXT_COLOR, BG_COLOR);
    tft.setTextSize(1);
    tft.print("mi");
    lastUnit = 0;
  }
}

void drawSpeedo(uint16_t Speed, uint8_t SpeedUnit, uint16_t MaxSpeed){
  if (SpeedUnit == 1){
    units = " km/h";
  }
  else{
    units = " mph ";
    Speed = Speed * 0.6213;
    MaxSpeed = MaxSpeed * 0.6213;
  }
  ringMeter((int)Speed, 0, MaxSpeed, 24, 8, 56, units, 4, 5);
}
  
void drawWarning (bool on) {
    if(on){
//      tft.drawRoundRect( p,  q,  l,  h, 3, ST77XX_CYAN);
//      tft.fillRoundRect(p+1,  q+1, l-2,  h-2, 2, ST77XX_WHITE);
    
//      tft.drawTriangle(p+l/2, q+4, p+l/2-12, q+h-16, p+l/2+12, q+h-16, ST77XX_RED);
      tft.fillTriangle(pl, q, pl-12, q+h, pl+12, q+h,ST77XX_RED);
    
      tft.setCursor(pl-5,q+h-15);
      tft.setTextColor( ST77XX_WHITE);
      tft.setTextSize(2);
      tft.print("!");
    
//      tft.setCursor(p+l/2-10, q+h-13);
//      tft.setTextColor( ST77XX_BLACK);
//      tft.setTextSize(1);
//      tft.print("com.error");
    }
    else{
//      tft.drawRoundRect( p,  q,  l,  h, 3, ST77XX_BLACK);
                            tft.fillTriangle(pl, q, pl-12, q+h, pl+12, q+h,ST77XX_BLACK);

    }
    
} 
void drawTemp(int Temp, uint8_t TempUnit){
  
  if (TempUnit == 2||Temp==initialTemp){
    tft.setCursor(0 , height - fontHeight);
    tft.setTextColor(TXT_COLOR, BG_COLOR);
    tft.setTextSize(1);
    tft.print(initialTempValue);
  }
  else{
    if (TempUnit == 1){
      unit = "C   "; // 3 intervals to draw over longer strings
    }else{
      Temp=Temp*1.8+320;
      unit = "F   "; // 3 intervals to draw over longer strings
    }
    tempMantis = Temp/10;
    tempExp = abs(Temp)%10;
    printTemp = " " + (String)tempMantis + "." + (String)tempExp+ char(247) + unit;
    tft.setCursor(0 , height - fontHeight);
    tft.setTextColor(TXT_COLOR, BG_COLOR);
    tft.setTextSize(1);
    tft.print(printTemp);
  }
  
}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme, byte inc)
{
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option

  x += r; y += r;   // Calculate coords of centre of ring

  int w = r / 4;    // Width of outer ring is 1/4 of radius

  int angle = 150;  // Half the sweep angle of meter (300 degrees)

  int text_colour = 0; // To hold the text colour

  int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

  byte seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
  //byte inc = 10; // Draw segments every 5 degrees, increase to 10 for segmented ring

  // Draw colour blocks every inc degrees
  for (int i = -angle; i < angle; i += inc) {

    // Choose colour from scheme
    int colour = 0;
    switch (scheme) {
      case 0: colour = _RED; break; // Fixed colour
      case 1: colour = _GREEN; break; // Fixed colour
      case 2: colour = _BLUE; break; // Fixed colour
      case 3: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
      case 4: colour = rainbow(map(i, -angle, angle, 63, 127)); break; // Green to red (high temperature etc)
      case 5: colour = rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
      case 6: colour = rainbow(map(i, -angle, angle, 92, 127)); break; // Yellow to red
      default: colour = _BLUE; break; // Fixed colour
    }

    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v) { // Fill in coloured segments with 2 triangles
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
      text_colour = colour; // Save the last colour drawn
    }
    else // Fill in blank segments
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, BG_GREY);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, BG_GREY);
    }
  }

  char buf[4] = {0};
  int xx=x;
  int yy=y;
  if(value<10){
    sprintf(buf, " %d ", value);
    x+=12;
  }
  else if(value<100){
    sprintf(buf, " %d ", value);
    x+=7;
  }
  else{
    sprintf(buf, "%d", value);
    x+=12;
  }

  tft.setCursor(x - 30, y - 10);
  tft.setTextColor(TXT_COLOR, BG_COLOR);
  tft.setTextSize(2);
  tft.print(buf);
  tft.setCursor(xx - 18, yy+ 8);
  tft.setTextColor(TXT_COLOR, BG_COLOR);
  tft.setTextSize(1);
  tft.print(units);

  // Calculate and return right hand side x coordinate
  return x + r;
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
