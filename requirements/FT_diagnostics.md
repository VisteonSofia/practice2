## Diagnostics feature

#### Brief description
The feature enable configuration of the instrument cluster. Provides an services to read/write variables in EEPROM

#### Inputs
  DispSpeed (int [0;511])
  _serial_input (string)

#### Outputs
  _serial_output (string)
  version (char[4], size=4B [0;31],default = "0x1", ["0x1";"0xFF"], read=true, write=false) - provide information of current version of the data structure
  mvs_spd_max_speed (int, size=9b [32,40], default = 250, [0;511], read=T, write=T) - define speed maximum
  mvs_odo_range (int,size=17b [41,57], default = 0, [0;131071], read=T, write=T) - store odometer value
  mvs_spd_is_km (bool, default=true, read=T, write=T) - define distance metric -> true - km/h, false - mph
  mvs_temp_is_celsius (bool, default=true, read=T, write=T) - define temperature metric -> true - celsius, false - fahrenheit
  mvs_is_acoustic_on - availability of the acoustics -> true - available, false - not
  mvs_end - constant 0xFF, end of block

#### Functional behaviour
*  R1 Outputs shall be available in all master features 0.1s after power on
*  R2 The feature shall provide an ability to store/read parameters from the console
*  R3 Available services:
** 0x22 | Read data by Identifier
** 0x2F | Write data by Identifier
*  R4 Available services shall be displayed on Serial after "help" request
*  R5 Information message shall be available on Serial on unrecognizable input not later than 1s
*  R6 22 service shall provide current data frame in decimals
*  R7 mvs_spd_max_speed can be changed when DispSpeed = 0
*  R8 Data shall be write byte by byte
*  R9 When changing of value contradict requirement request shall be reject with 7F 22 (7F - Negative responce, 31 - Conditions not correct)
*  R10 Unexpected input in recognized request shall be rejected with 7F 31 (7F - Negative responce, 31 - Request out of range)

#### Peer Review Comments
