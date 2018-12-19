#define IPC
#define BLINKERS
#define DIAG
#define INT_SERIAL
#define BUZZER_PIN 4
#define ODOMETER
#define DEMO

#include "diag.h"
#include "diag_execution.h"
#include "EasyBuzzer.h"

#define PRESSED 0 //buttons state
#define RELEASED 1 //buttons state

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
#ifdef INT_SERIAL
  Serial.begin(115200);
  Serial.println("Ready, steady, go!");
  Serial.flush();
#endif

#ifdef DIAG
  loadConfig();
  delay(100);
  saveConfig();
#endif
  EasyBuzzer.setPin(BUZZER_PIN);
  EasyBuzzer.beep( 2000, 3);
}

void loop() {
#ifdef DEMO
  if(storage.mvs_cycles_in_demo){
    demo();
    storage.mvs_cycles_in_demo--;
    saveConfig();
  }
  else{
#endif
    blinker_state_machine();
    ipc_state_machine();
    if(stringComplete){
      diag_execution(inputString);
      inputString = "";
      stringComplete = false;
    } 
    speedo_state_machine();
    tm_state_machine();
    EasyBuzzer.update();
    temp_state_machine();
    delay(10);
 #ifdef DEMO
  }
 #endif
 
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
