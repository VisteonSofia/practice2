#include "ecu_extract.h"
#define DIAG_DEBUG
#define POT_PIN A0

uint16_t t,data;

void diag_execution(String input){
        Serial.println(input);
       input.trim();
       input.toLowerCase();
    if (input=="help"){
         Serial.println("Available services:");
         Serial.println("0x22 | Read data by Identifier");
         Serial.println("0x2F | Write data by Identifier");
    }
    else if(input.startsWith("22")){
        if(input.length()!=2){
            Serial.println("7F 31");
           return;
        }
        loadConfig();
        delay(100);
        Serial.print("62");
      //  int *base = (int*)(&storage.version);
        for (t=0; t<sizeof(storage); t++){
             Serial.print(" ");
             Serial.print(EEPROM.read(CONFIG_START + t));
        }
        Serial.println("");
    }
    else if(input.startsWith("2f")){
      if (input.length()<6){
        Serial.println("7F 31");
        return;
      } 
      input.remove(0,3);
      for(t=0; t<input.length();t++){
        if(!(isDigit(input[t])||isSpace(input[t]))){
           Serial.println("7F 31");
           return;
        }
      }
      
      data= input.substring(input.indexOf(" ")+1).toInt();
      input.remove(input.indexOf(" "));
      if(input.toInt()<1){
        Serial.println("Version is read only");
        Serial.println("7F 22");
        return;
      }
      if(data>255){
        Serial.println("You write byte by byte. Accepted values[0-255]");
        Serial.println("7F 22");
      }
      else if((input.toInt()==2||input.toInt()==3)&&PDU1_storage.DispSpeed!=0){
        Serial.print("Current speed:");
        Serial.println(PDU1_storage.DispSpeed);
        Serial.println("7F 22");
      }
      else{

        EEPROM.write(CONFIG_START + input.toInt(), data);
       Serial.print("6F ");
       Serial.print(input);
       Serial.print(" ");
       Serial.println(data);
       loadConfig();
          delay(100);
       #ifdef DIAG_DEBUG
         
         Serial.print("Max speed:");
         Serial.println(storage.mvs_spd_max_speed);
         Serial.print("Odo range:");
         Serial.println(storage.mvs_odo_range);
       #endif
      }
      
      
       
       
    }
    else{    
         Serial.println("11");
         Serial.println("I can't understand you :( Do you wanna 'help'?");
    }
 }
