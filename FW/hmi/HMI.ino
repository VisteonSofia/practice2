#ifdef HMI_OLD

#define HMI_STARTING_TIME 500 //0,5sec
#define HMI_STARTUP_TIME 5000 //5sec
#define HMI_IDLE_TIME 350 //0,35sec
#define HMI_TX_PIN 2
#define HMI_RX_PIN 3

#include <SoftwareSerial.h>
SoftwareSerial mySerial(HMI_RX_PIN,HMI_TX_PIN); // pin 2 = TX, pin 3 = RX (unused)

enum hmi_state {HMI_INIT, HMI_STARTUP, HMI_STARTED, HMI_IDLE, HMI_REFRESH} hmi_stateVariable;
uint32_t hmi_msCounts=0;
uint32_t hmi_prevMillis=0;
int i=0;
int bargraph=0;

void hmi_state_machine() {
 if(hmi_prevMillis!=millis()) {
  hmi_msCounts++;
  hmi_prevMillis=millis();
 }

switch (hmi_stateVariable){
  
  case HMI_INIT:
     mySerial.begin(9600); // set up serial port for 9600 baud
     hmi_stateVariable = HMI_STARTUP;
     hmi_msCounts=0;
  break;

  case HMI_STARTUP:
    if(hmi_msCounts >= HMI_STARTING_TIME)
    {
      mySerial.write(254); // move cursor to beginning of first line
      mySerial.write(128);

      mySerial.write("POWERED         "); 
      mySerial.write("      BY VISTEON");

      mySerial.write(254); // move cursor to beginning of first line
      mySerial.write(128);

      hmi_stateVariable = HMI_STARTED;
      hmi_msCounts = 0;
    }
     break; 
   
   case HMI_STARTED:
    if(hmi_msCounts >= HMI_STARTUP_TIME)
    {
      hmi_stateVariable = HMI_IDLE;
      hmi_msCounts = 0;
    }  
     break; 

  case HMI_IDLE:
    if(hmi_msCounts >= HMI_IDLE_TIME)
    {
      hmi_stateVariable = HMI_REFRESH;
      hmi_msCounts = 0;
    }
     break; 
     

    case HMI_REFRESH:
      float temperature = getHMItemperature();
      uint32_t tripmeter = getHMItripmeter();
      uint8_t hmispeed = getHMIspeed();
      char temp[32]={0};
      if (getHMItempStat())
        {
          sprintf(temp, "-----");
        }
        else {
          sprintf(temp,"%d.%d", (int)temperature,((int)(temperature *100)%100));
        }
      char trip[32]={0};
      sprintf(trip,"%d", tripmeter);
      mySerial.write(254); // move cursor to beginning of first line
      mySerial.write(128);
      mySerial.write(temp);
      mySerial.write((char)223);
      mySerial.write("C    ");
      
      mySerial.write(trip);
      mySerial.write("km ");
     
      mySerial.write(254); 
      mySerial.write(192);
      mySerial.write("                ");
      
      mySerial.write(254); 
      mySerial.write(192);
      
      bargraph = hmispeed/10;
      for (i=0;i<bargraph;i++)
        {
          mySerial.write((char)219);
        }
         
      hmi_stateVariable = HMI_IDLE;
      hmi_msCounts = 0;

      break;
    }
  }
#endif
