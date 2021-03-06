//#include "IF1.h"
#include <Wire.h>


#define LED_PIN 3

//#include <Adafruit_GFX.h>     // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <SPI.h>


// Color definitions
#define ST77XX_BLACK       0x0000  ///<   0,   0,   0
#define ST77XX_NAVY        0x000F  ///<   0,   0, 123
#define ST77XX_DARKGREEN   0x03E0  ///<   0, 125,   0
#define ST77XX_DARKCYAN    0x03EF  ///<   0, 125, 123
#define ST77XX_MAROON      0x7800  ///< 123,   0,   0
#define ST77XX_PURPLE      0x780F  ///< 123,   0, 123
#define ST77XX_OLIVE       0x7BE0  ///< 123, 125,   0
#define ST77XX_LIGHTGREY   0xC618  ///< 198, 195, 198
#define ST77XX_DARKGREY    0x7BEF  ///< 123, 125, 123
#define ST77XX_BLUE        0x001F  ///<   0,   0, 255
#define ST77XX_GREEN       0x07E0  ///<   0, 255,   0
#define ST77XX_CYAN        0x07FF  ///<   0, 255, 255
#define ST77XX_RED         0xF800  ///< 255,   0,   0
#define ST77XX_MAGENTA     0xF81F  ///< 255,   0, 255
#define ST77XX_YELLOW      0xFFE0  ///< 255, 255,   0
#define ST77XX_WHITE       0xFFFF  ///< 255, 255, 255
#define ST77XX_ORANGE      0xFD20  ///< 255, 165,   0
#define ST77XX_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define ST77XX_PINK        0xFC18  ///< 255, 130, 198


#define IPC_ADDRESS 8

#define HMI

void setup() {
  
  Serial.begin(115200);
  Wire.begin(IPC_ADDRESS);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event

}

void loop() {
#ifdef HMI
  hmi_state_machine();
#endif

}

void receiveEvent(int howMany) {
   //Serial.print("Received bytes: ");
  // Serial.println(howMany);         // print 

   //Serial.print("Expected bytes: ");
   //Serial.println(sizeof(PDU1_storage));         // print 
   if( howMany != sizeof(PDU1_storage) ) return;
   Wire.readBytes( (byte *) &PDU1_storage, howMany);

//   Serial.print("Received left blinker:");
//   Serial.print(PDU1_storage.HMI_left_blinker_state); 
//   Serial.print(" Received right blinker:");
//   Serial.println(PDU1_storage.HMI_right_blinker_state); 
//   
   
}
