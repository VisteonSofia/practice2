#define IPC
#define BLINKERS
#define DIAG
#define INT_SERIAL

#include "diag.h"

#define PRESSED 0 //buttons state
#define RELEASED 1 //buttons state

void setup() {
#ifdef INT_SERIAL
  Serial.begin(115200);
  Serial.println("Starting..");
  Serial.flush();
#endif
  
#ifdef DIAG
  loadConfig();
  delay(100);
#endif

  
}

void loop() {

  blinker_state_machine();
  ipc_state_machine();
  
  delay(10);

}
