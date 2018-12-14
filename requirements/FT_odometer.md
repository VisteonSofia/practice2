TO-DO:

* Describe briefly feature
* Define inputs, outputs, parameters
* define valid ranges
* request data structure space from SW architect e.g.
* Define sending frequency
* Define feature behaviour
* Define HMI relevant behaviour e.g. SNA/ERR behaviour, decimal places to show, decimal separator sybmol (, or .) , negative/positive sign visualization, position on the display etc
* Define state machine

1.	Odometer
1.1	Introduction
Odometer feature calculates and displays the total mileage. Unit could be meter or mile depending on diagnostic parameter. The value of the display also depends from this diagnostic parameter.

Cluster odometer value can be updated by VelocitySignal or via Diagnostics.

Information is displayed on TFT display.

1.2	Inputs / Outputs
DistanceUnit_cfg – parameter that manages which unit will be displayed (miles or km)
OdometerValue_cfg – parameter that defines the value of Odometer in 0.1 km. When DistanceUnit_cfg is set to mile, then this value is converted with specific coefficient before to be used.
Displayed_Odometer – value of Odometer, displayed on TFT together with unit(depends from DistanceUnit_cfg).
VelocitySignal – signal that defines the value of velocity
OdoReset – parameter which describes if odometer value shall be reset or not. It has value “TRUE” and “FALSE”.
1.3	Configurations
SRS_Odometer.001.R1
DistanceUnit_cfg shall be a 1 bit parameter with the following values:
0x00 – km
0x01 – mile
Default value is 0x00.

SRS_Odometer.002.R1
OdometerValue_cfg shall be a 24 bit parameter. Unit is km and resolution is 0.1 km.
Default value is 0.
2.4 Functional requirements
SRS_Odometer.003.R1
If DistanceUnit_cfg=0x00 and (VelocitySignal hasn’t been received after cluster wake up OR VelocitySignal has been received for first time after cluster wake up), then Displayed_Odometer value shall be equal to value of OdometerValue_cfg.

SRS_Odometer.004.R1
If DistanceUnit_cfg=0x00, then Displayed_Odometer unit shall be km.

SRS_Odometer.005.R1
If DistanceUnit_cfg=0x01 and (VelocitySignal hasn’t been received after cluster wake up OR VelocitySignal has been received for first time after cluster wake up), then Displayed_Odometer value shall be equal to value of OdometerValue_cfg*0.621371192.

SRS_Odometer.006.R1
If DistanceUnit_cfg=0x01, then Displayed_Odometer unit shall be miles.

SRS_Odometer.007.R1
Value of OdometerValue_cfg = Displayed_Odometer shall be saved on write request of OdometerValue_cfg OR if odometer value calculation has incremented with 0.1 km.

SRS_Odometer.008.R1
If VelocitySignal has been received after cluster wake up, then OdometerValue_cfg value shall be updated once per 0.1km.

SRS_Odometer.008.R1
OdoPUSH timer shall be equal to 5 seconds.

SRS_Odometer.009.R1
If Odometer button is pressed for more than OdoPUSH, then OdoReset=”TRUE”.

SRS_Odometer.010.R1
If Odometer button is not pressed for more than OdoPUSH, then OdoReset=”FALSE”.

SRS_Odometer.010.R1
If OdoReset=”TRUE”, then OdometerValue_cfg and Displayed_Odometer  shall be equal to 0.

SRS_Odometer.012.R1
If maximum value of Displayed_Odometer is reached, then maximum value shall be kept until OdoReset=”TRUE” OR OdometerValue_cfg is updated through diagnostics.

2.5	Breakdown mode
Please see SRS_Odometer.001 and SRS_Odometer.003 to check behavior of cluster if VelocitySignal is missing after cluster wake up.

SRS_Odometer.013.R1
If VelocitySignal has been received after cluster wake up and it became not received, then dashes shall be shown until VelocitySignal is received.
