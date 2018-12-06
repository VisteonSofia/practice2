#ifdef SPEEDOMETER

#define SD_DEBUG
#define SD_IDLE_TIME 300 //0,3sec
#define POT_PIN A0


enum sd_state {SD_INIT, SD_IDLE, SD_READING } sd_stateVariable;
uint32_t sd_msCounts=0;
uint32_t sd_prevMillis=0;

void spvl_state_machine() {
 if(sd_prevMillis!=millis()) {
  sd_msCounts+=millis()-sd_prevMillis;
  sd_prevMillis=millis();
 }

switch (sd_stateVariable){
  
  case SD_INIT:
     PDU1_storage.DispSpeed=SD_SNA;
     sd_stateVariable = SD_IDLE;
     sd_msCounts=0;
  break;

  case SD_IDLE:
    if(sd_msCounts >= SD_IDLE_TIME)
    {
      sd_stateVariable = SD_READING;
      sd_msCounts = 0;
    }
     break; 

    case SD_READING:
      PDU1_storage.DispSpeed= map(analogRead(POT_PIN), 0, 1023, 0, 160);
      sd_stateVariable = SD_IDLE;
      sd_msCounts = 0;
#ifdef SD_DEBUG
      Serial.print("Measured speed: ");
      Serial.print(PDU1_storage.DispSpeed);    
      Serial.println("km/h ");      
#endif
      break;
    }
  }
#endif
