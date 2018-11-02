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
* UC5 When a blinker button and hazzards button are pressed withing 50 ms Then hazzards takes priority.
* UC6 When multiple blinker buttons are pressed withing 50 ms Then last one takes priority.
* UC7 When hazzards button is pressed and hazzards are OFF Then activate the hazzards after end of current blinking cycle.
* UC8 When hazzards button is pressed and hazzards are ON Then deactivate the hazzards after end of current blinking cycle.
* UC9 When blinker is ON Then it shall play a sound until light is deactivated.
* UC10 When hazzards are ON and press a blinker button Then hazzards shall remain active.
* UC11 When both blinkers buttons are pressed in same time Then no action shall be triggered.

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
* RCP6 - RCP13 proposal: When a blinker is ON Than the other can be activated and deactivated in the initial blinker cycle after that blinkrs shall be OFF


#### Test Cases
* TC1 - normal behavior
  1. Check blinkers OFF at 0.1s
  2. Press/release left button at 0.2/0.3
  3. Check 2 active phases of left blinker
  4. Press/release left button at 1.5/0.6
  EB:
  1. _HMI_left_blinker_state_ active (0.25,0.85) U (1.25,1.85)
  2. _HMI_left_blinker_state_ deactiveted [0,0.25) U [0.85,1.25) U [1.85,3]
  
* TC2 - long press
  1. Check blinkers OFF at 3.1s
  2. Press/release left button at 3.2/4.3
  3. Check 2 active phases of left blinker
  4. Press/release left button at 4.31/4.315
  EB:
  1. _HMI_left_blinker_state_ active (3.25,3.85) U (4.25,4.85)
  2. _HMI_left_blinker_state_ deactiveted [3,3.25) U (3.85,4.25) U (4.85,6]
 
 * TC3 - deactivation at last ms
  1. Check blinkers OFF at 6.1s
  2. Press/release left button at 6.2/6.3
  3. Check 1 active phase of left blinker
  4. Press/release left button at 7.199/7.3
  EB:
  1. _HMI_left_blinker_state_ active (6.25,6.85)
  2. _HMI_left_blinker_state_ deactiveted [6,6.25) U (6.85,8]
  
  * TC4 - late deactivation, wait one more cycle (3ms tolerance)
  1. Check blinkers OFF at 8.1s
  2. Press/release left button at 8.2/8.3
  3. Check 2 active phases of left blinker
  4. Press/release left button at 9.253/9.3
  EB:
  1. _HMI_left_blinker_state_ active (9.25,9.85) U (10.25,10.85)
  2. _HMI_left_blinker_state_ deactiveted [9,9.25) U (9.85,10.25) U (10.85,11]
  
  * TC5 - switch at last ms
  1. Check blinkers OFF at 11.1s
  2. Press/release left button at 11.2/11.3
  3. Check 1 active phase of left blinker
  4. Press/release right button at 12.199/12.3
  5. Check 2 active phases of right blinker
  6. Press/release right button at 13.3/13.305
  EB:
  1. _HMI_left_blinker_state_ active (11.25,11.85)
  2. _HMI_left_blinker_state_ deactiveted [11,11.25) U (11.85,15]
  3. _HMI_right_blinker_state_ active (12.249,12.849) U (13.249,13.849)
  4. _HMI_right_blinker_state_ deactiveted [11,12.249) U (12.849,13.249) U (13.849,15]
  
  * TC6 - Right ON and OFF in left operational cycle
  1. Check blinkers OFF at 15.1s
  2. Press/release left button at 15.2/15.3
  3. Check 2 active phases of blinker
  4. Press/release right button at 16.253/16.3
  5. Press/release right button at 16.5/16.6
  EB:
  1. _HMI_left_blinker_state_ active (15.25,15.85) U (16.25,16.85)
  2. _HMI_left_blinker_state_ deactiveted [15,15.25) U (15.85,16.25) U (16.85,18]
  3. _HMI_right_blinker_state_ deactiveted [15,18]
  
   * TC7 - late switch, wait one more cycle (3ms tolerance)
  1. Check blinkers OFF at 18.1s
  2. Press/release left button at 18.2/18.3
  3. Check 2 active phases of blinker
  4. Press/release right button at 19.253/19.3
  5. Check 1 active phases of right blinker
  6. Press/release right button at 20.5/20.6
  EB:
  1. _HMI_left_blinker_state_ active (18.25,18.85) U (19.25,19.85)
  2. _HMI_left_blinker_state_ deactiveted [18,18.25) U (18.85,19.25) U (19.85,22]
  3. _HMI_right_blinker_state_ active (20.25,20.85)
  4. _HMI_right_blinker_state_ deactiveted [18,20.25) U (20.85,22]
  
  * TC8 - double press right
  1. Check blinkers OFF at 22.1s
  2. Press/release right button at 22.2/22.21
  3. Press/release right button at 22.225/22.3
  4. Check 1 active phases of right blinker
  5. Press/release right button at 23.2/23.3
  EB:
  1. _HMI_right_blinker_state_ active (22.275,22.875)
  2. _HMI_right_blinker_state_ deactiveted [22,22.275) U (22.875,24]
  
   * TC9 - hazzards priority
  1. Check blinkers OFF at 24.1s
  2. Press/release hazzards button at 24.2/24.21
  3. Press/release right button at 24.225/24.3
  4. Check 1 active phases of hazzards
  5. Press/release hazzards button at 25.1/25.3
  EB:
  1. _HMI_left_blinker_state_ active (24.25,24.85)
  2. _HMI_left_blinker_state_ deactiveted [24,24.25) U (24.85,26]
  3. _HMI_right_blinker_state_ active (24.25,24.85)
  4. _HMI_right_blinker_state_ deactiveted [24,24.25) U (24.85,26]
  
  * TC10 - last one priority
  1. Check blinkers OFF at 26.1s
  2. Press/release left button at 26.2/26.21
  3. Press/release right button at 26.225/26.3
  4. Check 1 active phases of right blinker
  5. Press/release right button at 27.1/27.3
  EB:
  1. _HMI_right_blinker_state_ active (26.275,26.875)
  2. _HMI_right_blinker_state_ deactiveted [26,26.275) U (26.875,28]

* TC11 - Hazzards normal operation
  1. Check blinkers OFF at 28.1s
  2. Press/release left button at 28.2/28.3
  3. Check 1 active phases of left blinker
  4. Press/release hazzards button at 28.5/28.6
  5. Check 1 active phases of hazzards
  6. Press/release left button at 29.5/29.6
  7. Check 2 active phases of hazzards
  8. Press/release hazzards button at 31.5/31.6
  EB:
  1. _HMI_left_blinker_state_ active (28.25,28.85) U (29.25,29.85) U (30.25,30.85) U (31.25,31.85)
  2. _HMI_left_blinker_state_ deactiveted [28,28.25) U (28.85,29.25) U (29.85,30.25) U (30.85,31.25) U (31.85,33]
  3. _HMI_right_blinker_state_ active (29.25,29.85) U (30.25,30.85) U (31.25,31.85)
  4. _HMI_right_blinker_state_ deactiveted [28,29.25) U (29.85,30.25) U (30.85,31.25) U (31.85,33]
  
 * TC12 - Both blinkers at same time
  1. Check blinkers OFF at 33.1s
  2. Press/release left button at 33.2/33.3
  3. Press/release right button at 33.2/33.3
  4. Check 1 active phases of left blinker
  5. Check blinkers OFF at 33.6s
  EB:
  1. _HMI_left_blinker_state_ deactiveted [33,35]
  2. _HMI_right_blinker_state_ deactiveted [33,35]


#### Skill Review comments
not available

#### State diagram

![alt text](https://github.com/VisteonSofia/practice2/raw/master/requirements/img/blinkers_state_machine.PNG "Blinkers")

