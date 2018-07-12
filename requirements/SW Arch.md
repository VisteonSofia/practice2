1. Overview
This document describes minimum high level design information and associated views to be captured for a project or a SW platform. 
This document is intended to describe the Software Architecture of the practice2 project


2.Software high level design

- The System Software shall be separated by two independted software modules - one for the primay software,
called the application layer and one for the graphic interface, called the HMI
- Within each individual software module interface signals shall be defined.
- Interfaces are defined as input signals, output signals, internal 

  2.1 Design constraints
  
  Cricital resourse esimations:
  - Memory correct allocation 
  - CPU 1 bandwidth 
  - CPU 2 bandwidth
  - IPC bandwidth utilization (inter processor/device communication)
  - Time constraints 
  
  2.2.Design decisions
  
  - Two processors shall be used 
    *CPU 1: CPU_APP 
    *CPU 2: CPU_HMI
  - Memory 
    * FLASH - 12617260 bytes (12.61726 megabytes)
    * RAM - 37596002 bytes (37.596002 megabytes)
    
    
  2.3 Design allocation
    
