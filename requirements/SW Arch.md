Software Architecture specification 

1. Overview

This document describes minimum high level design information and associated views to be captured for a project or a SW platform. 
This document is intended to describe the Software Architecture of the practice2 project.

![alt text](https://raw.githubusercontent.com/VisteonSofia/practice2/ttsvetk1/ArchSw.jpg)

2.Software high level design

The System Software shall be separated by two independent software modules - one for the primary software,
called the application layer and one for the graphic interface, called the HMI. Within each individual software module communication layer shall be defined. A datastructure shall define the different types of interfaces used in the communication layer.

  2.1 Design constraints
  
  Cricital resourse esimations:
  - Memory allocation 
  - CPU 1 bandwidth 
  - CPU 2 bandwidth
  - IPC bandwidth utilization (inter processor/device communication)
  - Time constraints 
  
  2.2.Design decisions
    
  2.2.1 Communication 
    
  2.2.1.1 Protocol IPC
    
  IS1 and IS2 shall be connected via I²C protocol. A datastructue exchange shall be defined. The data shall be exchanged one-         directional from IS1 to IS2, where a feedback channel is defined from IS2 to IS1.  The communication from IS1 to IS2 is exchanged with 11bits messages, where the frame will be separated as following:
      Bit1 - ID of the frame
      Bit2-Bit10 - payload
      Bit11 - CRC counter used for verification/authenticity of the frame.
      Bit-rate - up to 1 Mbit/s 
    
  2.2.1.1 IF1 and IF2 abstraction mechanism
      
  2.2.2 Watchdog mechanism
         
     A watchdog timer (COP timer) shall be defined. The watchdog timer shall be used to prevent from elapsing or "timing out". If, due to a hardware fault or program error, the cluster fails to reset the watchdog, the timer will elapse and generate a timeout signal. The timeout signal is used to initiate corrective action or actions. The corrective actions typically include placing the system in a safe state and restoring normal system operation. The watchdog and CPU may share a common clock signal, or they may have independent clock signals.
     
  2.2.3 EEPROM structure (Diagnostics)
   
   
  2.2.4 SNA behaviour
   
   

    
    
    
