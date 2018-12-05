#ifdef HMI

float getHMItemperature()
{ 
  return ((float)(PDU1_storage.AmbientTemp - 999)/10);
  }
bool getHMItempStat()
{
  return (PDU1_storage.AmbientTemp == AT_SNA);
  }
uint32_t getHMItripmeter()
{ 
  return (PDU1_storage.DispDist/100);
  }
uint8_t getHMIspeed()
{ 
  return (PDU1_storage.DispSpeed);
  }

 #endif
