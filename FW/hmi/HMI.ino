#define HMI_STARTING_TIME 500 //0,5sec
#define HMI_STARTUP_TIME 5000 //5sec
#define HMI_IDLE_TIME 350 //0,35sec
#define HMI_TX_PIN 2
#define HMI_RX_PIN 3
#include "ecu_extract.h"
#include "IF1.h"
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

#define IPC_ADDRESS 8

#include <SoftwareSerial.h>
SoftwareSerial mySerial(HMI_RX_PIN,HMI_TX_PIN); // pin 2 = TX, pin 3 = RX (unused)

static const char _sclk = 13;
static const char  _mosi = 11;
static const char _cs = 10;
static const char _dc = 9;
static const char _rst = 8;

Adafruit_ST7735 tft = Adafruit_ST7735(_cs, _dc, _rst);

enum hmi_state {HMI_INIT, HMI_STARTUP, HMI_STARTED, HMI_IDLE, HMI_REFRESH} hmi_stateVariable;
uint32_t hmi_msCounts=0;
uint32_t hmi_prevMillis=0;
int i=0;
int bargraph=0;

void hmi_state_machine() {
 if(hmi_prevMillis!=millis()) {
  hmi_msCounts++;
  hmi_prevMillis=millis();
 }

switch (hmi_stateVariable){
  
  case HMI_INIT:
     mySerial.begin(9600); // set up serial port for 9600 baud
     hmi_stateVariable = HMI_STARTUP;
     hmi_msCounts=0;
  break;

  case HMI_STARTUP:
    if(hmi_msCounts >= HMI_STARTING_TIME)
    {
      mySerial.write(254); // move cursor to beginning of first line
      mySerial.write(128);

      mySerial.write("POWERED         "); 
      mySerial.write("      BY VISTEON");

      mySerial.write(254); // move cursor to beginning of first line
      mySerial.write(128);

      hmi_stateVariable = HMI_STARTED;
      hmi_msCounts = 0;
    }
     break; 
   
   case HMI_STARTED:
    if(hmi_msCounts >= HMI_STARTUP_TIME)
    {
      hmi_stateVariable = HMI_IDLE;
      hmi_msCounts = 0;
    }  
     break; 

  case HMI_IDLE:
    if(hmi_msCounts >= HMI_IDLE_TIME)
    {
      hmi_stateVariable = HMI_REFRESH;
      hmi_msCounts = 0;
    }
     break; 
     

    case HMI_REFRESH:
      //uint32_t temperature = getTemperature();
      uint32_t Odometer = getOdometer();
      uint16_t Speed = getSpeed();
      uint16_t MaxSpeed = getMaxSpeed();
      uint8_t SpeedUnit = getSpeedunit();
      uint8_t BlinkerLeft = getBlinkerLeft();
      uint8_t BlinkerRight = getBlinkerRight();
      int Temp = getTemperature();
      uint8_t TempUnit =  getTempUnit();

      drawBlinkers(BlinkerLeft, BlinkerRight);
      drawSpeedo();
      
      /*
      char temp[32]={0};
      if (getHMItempStat())
        {
          sprintf(temp, "-----");
        }
        else {
          sprintf(temp,"%d.%d", (int)temperature,((int)(temperature *100)%100));
        }
      char trip[32]={0};
      sprintf(trip,"%d", tripmeter);
      mySerial.write(254); // move cursor to beginning of first line
      mySerial.write(128);
      mySerial.write(temp);
      mySerial.write((char)223);
      mySerial.write("C    ");
      
      mySerial.write(trip);
      mySerial.write("km ");
     
      mySerial.write(254); 
      mySerial.write(192);
      mySerial.write("                ");
      
      mySerial.write(254); 
      mySerial.write(192);
      
      bargraph = hmispeed/10;
      for (i=0;i<bargraph;i++)
        {
          mySerial.write((char)219);
        }
      */ 
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
     tft.fillTriangle( 160-f,a,160-e,b+c/2,160-f,b+c+g,ST77XX_GREEN);
    }
    else {
     tft.fillRect(160-f-d, b, d, c, ST77XX_BLACK);
     tft.fillTriangle( 160-f,a,160-e,b+c/2,160-f,b+c+g,ST77XX_BLACK);
   }
  
}

void drawODO(){
}

void drawSpeedo(uint16_t Speed, uint8_t SpeedUnit, uint16_t MaxSpeed){
  if (SpeedUnit == 1){
    char *units = " km/h"
  }
  else
    char *units = " mph"
  ringMeter((int)Speed, 0, MaxSpeed, 32, 16, 52, units, 4, 10);
}

void drawTemp(){
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

  char buf[10] = {0};
  sprintf(buf, " %d ", value);

  tft.setCursor(x - 30, y - 10);
  tft.setTextColor(TXT_COLOR, BG_COLOR);
  tft.setTextSize(3);
  tft.print(buf);

  tft.setCursor(x - 15, y + 40);
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
