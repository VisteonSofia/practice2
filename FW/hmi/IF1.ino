#ifdef HMI
int getTemperature()
{ 
  return (PDU1_storage.DispTemp);
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
#endif
