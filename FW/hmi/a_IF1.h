uint8_t prevAlive;

//#include "a_IF1"
int getTemperature();
uint8_t getTempUnit();
uint32_t getOdometer();
uint16_t getSpeed();
uint16_t getMaxSpeed();
uint8_t getSpeedUnit();
uint8_t getBlinkerLeft();
uint8_t getBlinkerRight();
bool pduStatus();
uint8_t stringChecksum();
int getTemperature()
{ 
  return (PDU1_storage.DispTemp);
  }
uint8_t getTempUnit()
{ 
  return (PDU1_storage.UnitTemp);
  }
uint32_t getOdometer()
{ 
  return (PDU1_storage.DispDist);
  }
uint16_t getMaxSpeed()
{ 
  return (PDU1_storage.MaxSpeed);
  }
uint16_t getSpeed()
{ 
  return (PDU1_storage.DispSpeed);
  }
uint8_t getSpeedUnit()
{ 
  return (PDU1_storage.UnitSpeed);
  }
uint8_t getBlinkerLeft()
{ 
  return (PDU1_storage.HMI_left_blinker_state);
  }
uint8_t getBlinkerRight()
{ 
  return (PDU1_storage.HMI_right_blinker_state);
  }

bool pduStatus()
{ 
  
  if(PDU1_storage.crc!=stringChecksum())
    return false;
  if(PDU1_storage.alive==prevAlive){
    prevAlive=PDU1_storage.alive;
    return false;
  }
  prevAlive=PDU1_storage.alive;
  return true;
}
uint8_t stringChecksum(){
    uint8_t payload[sizeof(PDU1_storage)-1];
      //copy struct to variable array
      memcpy(payload,&PDU1_storage,sizeof(PDU1_storage));
      uint16_t c = 0;
      //send each item of struct, now contained in payload array
      for(int t=0;t < sizeof(payload);t++)
      {
            c ^= payload[t];       
      }
    return c;
}
