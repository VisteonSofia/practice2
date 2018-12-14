#ifdef TRIPMETER
#include <EEPROM.h>
  
#define TM_DEBUG
#define TM_IDLE_TIME 1000 //1sec
#define TM_RESET_BUTTON 7

enum tm_state {TM_INIT, TM_IDLE, TM_CALCULATING} tm_stateVariable;
uint32_t tm_msCounts=0;
uint32_t tm_prevMillis=0;
bool prevButtonState = 1;
float dist_temp;

void tm_state_machine() 
{
  
 if (tm_prevMillis != millis()) {
      tm_msCounts += (millis() - tm_prevMillis);
          
      tm_prevMillis = millis();
    }



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
       if(digitalRead(TM_RESET_BUTTON)== PRESSED && prevButtonState== RELEASED) {      
          tm_stateVariable = TM_INIT;
          tm_msCounts = 0;
          break;
        } else prevButtonState=digitalRead(TM_RESET_BUTTON);
        
      if(tm_msCounts >= TM_IDLE_TIME)
      {
        tm_stateVariable = TM_CALCULATING;
        tm_msCounts = 0;
      }
    break; 

    case TM_CALCULATING:
    /*  if (PDU1_storage.DispSpeed==SD_SNA)
      {
          tm_stateVariable = TM_IDLE;
          tm_msCounts = 0;
          
          break; // abort if no speed is available from the corresponding feature
      }*/
        
       dist_temp += ((uint32_t)((uint32_t)PDU1_storage.DispSpeed * TM_IDLE_TIME)/3600); // Multiplying the speed value and the time value and normalizing the value, adding the result to the previous value
      
    
    if (dist_temp > 1)
    {
       dist_temp--;
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
        Serial.print("m; Driven distance since last save: ");
        Serial.print(dist_temp);
        Serial.println("km");      
      #endif
    break;
  }
}
#endif
