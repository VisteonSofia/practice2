Blinkers feature

Brief description
The feature manages the blinker activation/deactivation and signals HMI / acoustics functions for visual / acoustic notification of the user.

Inputs
int_left_blinker_btn_press  (bool, 0 means not pressed, 1 means pressed)
int_right_blinker_btn_press (bool, 0 means not pressed, 1 means pressed)

Outputs
HMI_left_blinker_state (bool, IF1 relevant, 0 means blinker not shown, 1 means show blinker)
HMI_right_blinker_state (bool, IF1 relevant, 0 means blinker not shown, 1 means show blinker)

Parameters
Blinkers_accoustics_priority (default = low (0), max = 10, non diag accessible)
Blinkers_accoustics_on_duration (default = 50ms, non diag accessible)
Blinkers_accoustics_off_duration (default = 50ms, non diag accessible)
Blinkers_accoustics_on_freq (default = 2KHz, non diag accessible)
Blinkers_accoustics_off_duration (default = 1.4Khz, non diag accessible)
Blinkers_on_duration (default = 600ms, non diag accessible)
Blinkers_off_duration (default = 400ms, non diag accessible)

* R1 The feature will run every 50ms
* R2 The feature controls two separate blinkers - left and right
* R3 The blinkers are activated with dedicated hardware buttons. If the respective blinker is OFF, the button activates it, if the respective blinker is BLINKING when the respective blinker button is pressed - it is deactivated
* R4 "Hazzard notification"function shall be available, if time permits. The function activates both blinkers at the same time.
* R5 The blinkers shall be accompanied with synchronised audiable clue
* R6 The blinkers cannot be activated at the same time, except in "hazard notification mode"
* R7 Blink cycle shall not be interrupted
* R8 Blink cycles are 600ms on and 400ms off, customiseable via parameters Blinkers_on_duration and Blinkers_off_duration
* R9 Fail behaviour: HMI shall set both blinkers to constantly lit upon TMO/SNA conditions of the HMI_left_blinker_state/HMI_right_blinker_state signals

