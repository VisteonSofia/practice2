
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#include "EasyBuzzer.h"
#define BUZZER_PIN 4

// These are the connections from the UNO to the display
#define sclk 13  // Don't change
#define mosi 11  // Don't change
#define cs   10  // If cs and dc pin allocations are changed then 
#define dc   9   // comment out #define F_AS_T line in "Adafruit__FAST.h"
#define rst  8  // Can alternatively connect this to the Arduino reset
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

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

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5
#define YELLOW2RED 6

//#define USE_LIGHT_SCHEME

#ifdef USE_LIGHT_SCHEME
#define BG_COLOR _WHITE
#define TXT_COLOR _BLACK
#define BG_GREY _LIGHTGREY
#else
#define BG_COLOR _BLACK
#define TXT_COLOR _WHITE
#define BG_GREY _GREY
#endif

uint32_t runTime = -99999;       // time for next update
int reading = 0; // Value to be displayed

void setup(void) {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(3);
  tft.fillScreen(BG_COLOR);

  EasyBuzzer.setPin(BUZZER_PIN);
}

int ADC_PT1(int adc_pin) {
  static int ADC_prev;
  int rawADC=analogRead(adc_pin);
  rawADC = (0.4 * rawADC) +(0.6 * ADC_prev);  
  //save old values
  ADC_prev = rawADC;
  Serial.println(rawADC);
  return rawADC;
}

void loop() {
  EasyBuzzer.update();

  if (millis() - runTime >= 100L) { // Execute every 100ms
    runTime = millis();
    EasyBuzzer.beep(map(ADC_PT1(A0), 0, 1024, 200, 500));
    reading = map(ADC_PT1(A0), 0, 1024, 0, 260);
    ringMeter(reading, 0, 260, 24 , 8 , 60 , " km/h", BLUE2RED, 10); // Draw analogue meter
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
