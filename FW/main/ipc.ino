#ifdef IPC

#include <Wire.h>

//#define IPC_DEBUG
#define IPC_IDLE_TIME 50 //0,05sec
#define IPC_ADDRESS 8


enum ipc_state {IPC_INIT, IPC_IDLE, IPC_TRANSMIT } ipc_stateVariable;
uint32_t ipc_msCounts=0;
uint32_t ipc_prevMillis=0;

void ipc_state_machine() {
 if(ipc_prevMillis!=millis()) {
  ipc_msCounts++;
  ipc_prevMillis=millis();
 }

switch (ipc_stateVariable){
  
  case IPC_INIT:
     Wire.begin(); // pull up of the I2C
     ipc_stateVariable = IPC_IDLE;
     ipc_msCounts=0;
  break;

  case IPC_IDLE:
    if(ipc_msCounts >= IPC_IDLE_TIME)
    {
      ipc_stateVariable = IPC_TRANSMIT;
      ipc_msCounts = 0;
    }
     break; 

    case IPC_TRANSMIT:
      Wire.beginTransmission(IPC_ADDRESS); // to the next device 
      Wire.write((byte *)&PDU1_storage,sizeof(PDU1_storage));
      Wire.endTransmission();    // stop transmitting
      ipc_stateVariable = IPC_IDLE;
      ipc_msCounts = 0;
    break;
    }
  }
#endif