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
  - IPC bandwidth utilization (inter processor/device communication)
  - Time constraints 
  
  2.2.Design decisions
    
   2.2.1 Communication 
    
   (1) Protocol IPC
    
   A datastructure exchange shall be defined. The data shall be exchanged as one-directional from IF1. The data shall be sent as packages from the application software. Each package is visualized as following:
   
   ![alt text](https://raw.githubusercontent.com/VisteonSofia/practice2/ttsvetk1/frame.jpg)
  
   (1) ID of the frame
   (2) payload
   (3) Message Counter
   (4) CRC (Cyclic Redundancy Check)- An error-detecting code to detect accidental changes to raw data 
    
   2.2.2 Watchdog mechanism
         
   A watchdog timer (COP timer) shall be defined. The watchdog timer shall be used to prevent from elapsing or "timing out". If, due to    a hardware fault or program error, the cluster fails to reset the watchdog, the timer will elapse and generate a timeout signal. The    timeout signal is used to initiate corrective action or actions. The corrective actions typically include placing the system in a        safe state and restoring normal system operation. The watchdog and CPU may share a common clock signal, or they may have independent    clock signals.
     
   2.2.3 EEPROM structure (Diagnostics)
   
   
   2.2.4 SNA behaviour
   
   

    
    
    
