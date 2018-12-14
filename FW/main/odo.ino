#ifdef ODOMETER
#include <EEPROM.h>
  
// #define TM_DEBUG
#define TM_IDLE_TIME 1000 //1sec
#define TM_RESET_BUTTON 7

enum tm_state {TM_INIT, TM_IDLE, TM_CALCULATING} tm_stateVariable;
uint32_t tm_msCounts=0;
uint32_t tm_prevMillis=0;
bool prevButtonState = 1;
uint16_t dist_temp;

void tm_state_machine() 
{
  
  if (tm_prevMillis != millis()) {
    tm_msCounts += (millis() - tm_prevMillis);
        
    tm_prevMillis = millis();
  }

  if(digitalRead(TM_RESET_BUTTON)== PRESSED && prevButtonState== RELEASED) {      
    tm_stateVariable = TM_IDLE;
    PDU1_storage.DispDist=storage.mvs_odo_range=dist_temp=0;
    saveConfig();
    tm_msCounts = 0;
  } 
  prevButtonState=digitalRead(TM_RESET_BUTTON);


  switch (tm_stateVariable)
  {
    case TM_INIT:
      pinMode(TM_RESET_BUTTON, INPUT_PULLUP);
      prevButtonState=digitalRead(TM_RESET_BUTTON);
      PDU1_storage.DispDist=storage.mvs_odo_range;
      tm_stateVariable = TM_IDLE;
      tm_msCounts=0;
    break;

    case TM_IDLE:
        
      if(tm_msCounts >= TM_IDLE_TIME)
      {
        tm_stateVariable = TM_CALCULATING;
        tm_msCounts = 0;
      }
    break; 

    case TM_CALCULATING:

       dist_temp += ((uint32_t)((uint32_t)PDU1_storage.DispSpeed * TM_IDLE_TIME)/3600);
      
    
    if (dist_temp > 1000)
    {
       dist_temp-=1000;
       storage.mvs_odo_range++;
       PDU1_storage.DispDist++;
       saveConfig();
    }
       
     
     /* if (PDU1_storage.DispDist>=999)
      {
        PDU1_storage.DispDist = 0;
      }*/
      
      tm_stateVariable = TM_IDLE;
      tm_msCounts = 0;
      
      #ifdef TM_DEBUG
        Serial.print("Driven distance: ");
        Serial.print(PDU1_storage.DispDist); 
        Serial.print("km; Driven distance since last save: ");
        Serial.print(dist_temp);
        Serial.println("m");      
      #endif
    break;
  }
}
#endif
