#include <OneWire.h>
// #define TP_DEBUG
OneWire  ds(2);
#define tp_IDLE_TIME 1250 // maybe 750ms is enough, maybe not


enum temp_state {TP_INIT, TP_IDLE, TP_READING, TP_DELAYED_READING } tp_stateVariable;
uint32_t tp_msCounts=0;
uint32_t tp_prevMillis=0;
char *prev_state = "TP_DELAYED_READING";

byte ii;
byte present = 0;
byte type_s;
byte data_temp[12];
byte addr[8];
float celsius, fahrenheit;

void temp_state_machine() {
 if(tp_prevMillis!=millis()) {
  tp_msCounts+=millis()-tp_prevMillis;
  tp_prevMillis=millis();
 }

switch (tp_stateVariable){
  
  case TP_INIT:
      PDU1_storage.DispTemp= -600;
     PDU1_storage.UnitTemp= storage.mvs_temp_is_celsius;
     tp_stateVariable = TP_IDLE;
          tp_msCounts=0;
  break;

  case TP_IDLE:
    if(tp_msCounts >= tp_IDLE_TIME)
    {
     if(prev_state=="TP_DELAYED_READING"){
      tp_stateVariable = TP_READING;
     }
     else{
      tp_stateVariable = TP_DELAYED_READING;
     }
            tp_msCounts = 0;
    }
    
     break; 

    case TP_READING:
                       
             if ( !ds.search(addr)) {
      //        Serial.println("No more addresses.");
      //        Serial.println();
              ds.reset_search();
              delay(250);
              return;
            }
            
      //      Serial.print("ROM =");
      //      for( i = 0; i < 8; i++) {
      //        Serial.write(' ');
      //        Serial.print(addr[i], HEX);
      //      }
          
      //      if (OneWire::crc8(addr, 7) != addr[7]) {
      //          Serial.println("CRC is not valid!");
      //          return;
      //      }
      //      Serial.println();
           
            // the first ROM byte indicates which chip
            switch (addr[0]) {
              case 0x10:
      //          Serial.println("  Chip = DS18S20");  // or old DS1820
                type_s = 1;
                break;
              case 0x28:
      //          Serial.println("  Chip = DS18B20");
                type_s = 0;
                break;
              case 0x22:
      //          Serial.println("  Chip = DS1822");
                type_s = 0;
                break;
              default:
      //          Serial.println("Device is not a DS18x20 family device.");
                return;
            } 
          
            ds.reset();
            ds.select(addr);
            ds.write(0x44, 1);        // start conversion, with parasite power on at the end
            
            tp_stateVariable = TP_IDLE;
            tp_msCounts = 0;
            prev_state="TP_READING";
            
            break;
      case TP_DELAYED_READING:
      // we might do a ds.depower() here, but the reset will take care of it.
      present = ds.reset();
      ds.select(addr);    
      ds.write(0xBE);         // Read Scratchpad
    
//      Serial.print("  Data = ");
//      Serial.print(present, HEX);
//      Serial.print(" ");
      for ( ii = 0; ii < 9; ii++) {           // we need 9 bytes
        data_temp[ii] = ds.read();
//        Serial.print(data_temp[ii], HEX);
//        Serial.print(" ");
      }
//      Serial.print(" CRC=");
//      Serial.print(OneWire::crc8(data_temp, 8), HEX);
//      Serial.println();

      if(data_temp[8]!=OneWire::crc8(data_temp, 8)){
          PDU1_storage.UnitTemp=2;
      }
      else{
          PDU1_storage.UnitTemp= storage.mvs_temp_is_celsius;
      }
    
      // Convert the data to actual temperature
      // because the result is a 16 bit signed integer, it should
      // be stored to an "int16_t" type, which is always 16 bits
      // even when compiled on a 32 bit processor.
      int16_t raw = (data_temp[1] << 8) | data_temp[0];
      if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data_temp[7] == 0x10) {
          // "count remain" gives full 12 bit resolution
          raw = (raw & 0xFFF0) + 12 - data_temp[6];
        }
      } else {
        byte cfg = (data_temp[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        //// default is 12 bit resolution, 750 ms conversion time
      }
      celsius = (float)raw / 16.0;
      celsius-=7.4;//correction
      fahrenheit = celsius * 1.8 + 32.0;
      if(storage.mvs_temp_is_celsius){
        PDU1_storage.DispTemp= celsius*10;
      }
      else{
        PDU1_storage.DispTemp= fahrenheit*10;
      }
      tp_stateVariable = TP_IDLE;
      #ifdef TP_DEBUG
        Serial.print("PDU temp: ");
        Serial.println(PDU1_storage.DispTemp);  
        Serial.println(PDU1_storage.UnitTemp);
        Serial.print("  Temperature = ");
        Serial.print(celsius);
        Serial.print(" Celsius, ");
        Serial.print(fahrenheit);
        Serial.println(" Fahrenheit");
       
       #endif
      tp_stateVariable = TP_IDLE;
      tp_msCounts = 0;
      prev_state="TP_DELAYED_READING";
      break;
      default:
         Serial.println("What are you doing here?");
    }
    
  }
