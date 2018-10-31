## Blinkers feature

#### Brief description
The feature manages the blinker activation/deactivation and signals HMI / acoustics functions for visual / acoustic notification of the user.

#### Inputs
_int_left_blinker_btn_press_  (bool, 0 means not pressed, 1 means pressed)

_int_right_blinker_btn_press_ (bool, 0 means not pressed, 1 means pressed)

_int_hazzard_btn_press_       (bool, 0 means not pressed, 1 means pressed)


#### Outputs
_HMI_left_blinker_state_  (bool, IF1 relevant, 0 means blinker not shown, 1 means show blinker)

_HMI_right_blinker_state_ (bool, IF1 relevant, 0 means blinker not shown, 1 means show blinker)


#### Parameters
_Blinkers_accoustics_priority_ (default = low (0), max = 10, non diag accessible)

_Blinkers_accoustics_on_duration_ (default = 50ms, non diag accessible)

_Blinkers_accoustics_off_duration_ (default = 50ms, non diag accessible)

_Blinkers_accoustics_on_freq_ (default = 2KHz, non diag accessible)

_Blinkers_on_duration_ (default = 600ms, non diag accessible)

_Blinkers_off_duration_ (default = 400ms, non diag accessible)


#### Functional behaviour
* R1 The feature will run every 50ms
* R2 The feature controls two separate blinkers - left and right
* R3 The blinkers are activated with dedicated hardware buttons. If the respective blinker is OFF, the button activates it, if the respective blinker is BLINKING when the respective blinker button is pressed - it is deactivated
* R4 "Hazzard notification"function shall be available, if time permits. The function activates both blinkers at the same time.
* R5 The blinkers shall be accompanied with synchronised audiable clue
* R6 The blinkers cannot be activated at the same time, except in "hazard notification mode"
* R7 Blink cycle shall not be interrupted
* R8 Blink cycles are 600ms on and 400ms off, customiseable via parameters Blinkers_on_duration and Blinkers_off_duration
* R9 Fail behaviour: HMI shall set both blinkers to constantly lit upon TMO/SNA conditions of the HMI_left_blinker_state/HMI_right_blinker_state

#### User cases
* UC1 When left button is pressed and hazzards are OFF Then activate the left blinker.
* UC2 When left button is pressed and right blinker is ON Then activate the left blinker after end of current blinking cycle.
* UC3 When left button is pressed and left blinker is ON Then deactivate the left blinker after end of current blinking cycle.
* UC4 When a button is double pressed withing 50 ms Then one press shall be triggered.
* UC5 When a blinker button and hazzards button is pressed withing 50 ms Then hazzards takes priority.
* UC6 When multiple blinker buttons are pressed withing 50 ms Then last one takes priority.
* UC7 When hazzards button is pressed and hazzards are OFF Then activate the hazzards after end of current blinking cycle.
* UC8 When hazzards button is pressed and hazzards are ON Then deactivate the hazzards after end of current blinking cycle.
* UC9 When blinker is ON Then it shall play a sound until light is deactivated.

#### Peer Review Comments
* PRC1 regarding R1:"The feature will run every 50ms" >>> Run does not defined expected action.
* PRC2 regarding R8:"Blink cycles are 600ms on and 400ms off, customiseable via parameters Blinkers_on_duration and Blinkers_off_duration" >>> Blinkers_on_duration and Blinkers_off_duration are parameters instead of contants but set/change are not defined.
* PRC3 regarding R9:"HMI shall set both blinkers to constantly lit upon TMO/SNA conditions of the HMI_left_blinker_state/HMI_right_blinker_state" >>> Sound behavior is not defined

#### Req. Clarification Proposal
* RCP1 regarding R1:"The feature will run every 50ms" >>> The feature shall provide update on the outputs HMI_left_blinker_state/HMI_right_blinker_state every 50 ms
* RCP2 regarding R9:"HMI shall set both blinkers to constantly lit upon TMO/SNA conditions of the HMI_left_blinker_state/HMI_right_blinker_state" >>> HMI shall set both blinkers and sound to constantly lit upon TMO/SNA conditions of the HMI_left_blinker_state/HMI_right_blinker_state
* RCP3 - RCP10 proposal: Wait 50 ms and provide an update on outputs in the end of current 50ms frame.
* RCP4 - RCP11 proposal: When a button is double pressed withing 50 ms Then one press shall be triggered.
* RCP5 - RCP12 proposal: Hazzards button has priority over blinking events.

#### Test Cases
* TC1 - normal behavior
  1. Check blinkers OFF at 0.1s
  2. Press/release left button at 0.2/0.3
  3. Check active 2 active phase of blinker
  4. Press/release left button at 1.5/0.6
  EB:
  1. _HMI_left_blinker_state_ active [0.25,0.85) U [1.25,1.85)
  2. _HMI_left_blinker_state_ deactiveted [0,0.25) U [0.85,1.25) U [1.85,3]
  
* TC2 - long press
  1. Check blinkers OFF at 3.1s
  2. Press/release left button at 3.2/4.3
  3. Check active 2 active phase of blinker
  4. Press/release left button at 4.31/4.315
  EB:
  1. _HMI_left_blinker_state_ active [3.25,3.85) U [4.25,4.85)
  2. _HMI_left_blinker_state_ deactiveted [3,3.25) U [3.85,4.25) U [4.85,6]
 
 * TC3 - deactivation at last ms
  1. Check blinkers OFF at 6.1s
  2. Press/release left button at 6.2/6.3
  3. Check active 1 active phase of blinker
  4. Press/release left button at 7.199/7.3
  EB:
  1. _HMI_left_blinker_state_ active [6.25,6.85)
  2. _HMI_left_blinker_state_ deactiveted [6,6.25) U [6.85,8]
  
  * TC3 - late deactivation, wait one more cycle
  1. Check blinkers OFF at 8.1s
  2. Press/release left button at 8.2/8.3
  3. Check active 2 active phase of blinker
  4. Press/release left button at 9.251/9.3
  EB:
  1. _HMI_left_blinker_state_ active [9.25,9.85) U [10.25,10.85)
  2. _HMI_left_blinker_state_ deactiveted [9,9.25) U [9.85,10.25)


#### Skill Review comments
not available

#### State diagram

![alt text](https://github.com/VisteonSofia/practice2/raw/master/requirements/img/blinkers_state_machine.PNG "Blinkers")

