
#define POT_PIN A0

//#define SD_DEBUG
#define SD_IDLE_TIME 150 //0,05sec
#define POT_PIN A0



enum sd_state {SD_INIT, SD_IDLE, SD_READING } sd_stateVariable;
uint32_t sd_msCounts=0;
uint32_t sd_prevMillis=0;
int current_speed[5]={0,0,0,0,0};
#define ARRAY_SIZE 5
uint16_t i,sum, weights_sum;
void speedo_state_machine() {
 if(sd_prevMillis!=millis()) {
  sd_msCounts+=millis()-sd_prevMillis;
  sd_prevMillis=millis();
 }

switch (sd_stateVariable){
  
  case SD_INIT:
     PDU1_storage.DispSpeed= 0;
     sd_stateVariable = SD_IDLE;
          sd_msCounts=0;
          
     for (i=weights_sum=0;i<ARRAY_SIZE;i++){
         weights_sum+=i+1;
     }
  break;

  case SD_IDLE:
    if(sd_msCounts >= SD_IDLE_TIME)
    {
      sd_stateVariable = SD_READING;
            sd_msCounts = 0;
    }
    
     break; 

    case SD_READING:
      for (i=sum=0;i<ARRAY_SIZE-1;i++){
        current_speed[i]=current_speed[i+1];
        sum+=current_speed[i]*(ARRAY_SIZE-i);  
      }
      current_speed[ARRAY_SIZE-1]=map(analogRead(POT_PIN), 0, 1023, 0, storage.mvs_spd_max_speed);
      sum+=current_speed[ARRAY_SIZE-1];
      sum/=weights_sum;
      PDU1_storage.DispSpeed= sum;
      PDU1_storage.UnitSpeed= storage.mvs_spd_is_km;
      PDU1_storage.MaxSpeed= storage.mvs_spd_max_speed;
      sd_stateVariable = SD_IDLE;
    #ifdef SD_DEBUG
      Serial.print("Measured speed: ");
      Serial.print(PDU1_storage.DispSpeed);  
      Serial.print("Max speed: ");
      Serial.println(storage.mvs_spd_max_speed);  
      Serial.println(PDU1_storage.UnitSpeed);
     // sd_msCounts = 0;
     #endif
      break;
    }
  }
