TO-DO:

Brief description
Define inputs, outputs, parameters
define valid ranges

request data structure space from SW architect e.g.

Define sending frequency
Define feature behaviour
Define HMI relevant behaviour e.g. SNA/ERR behaviour, decimal places to show, decimal separator sybmol (, or .) , negative/positive sign visualization, position on the display etc
Define state machine

Brief description
The feature shall provide activation/deactivation signals to HMI

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

The feature will run every 50ms
The feature controls two separate blinkers - left and right
The blinkers are activated with dedicated hardware buttons. If the respective blinker is OFF, the button activates it, if the respective blinker is on when button pressed, the button deactivates it
"Hazzard notification"function shall be available, if time permits. The function activates both blinkers at the same time.
The blinkers shall be accompanied with synchronised audiable clue
The blinkers cannot be activated at the same time, except in "hazard notification mode"
Blink cycle shall not be interrupted
Blink cycles are 600ms on and 400ms off

Fail behaviour: both blinkers constantly lit

