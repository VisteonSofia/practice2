TO-DO:

* Describe briefly feature
* Define inputs, outputs, parameters
* define valid ranges
* request data structure space from SW architect e.g.
* Define sending frequency
* Define feature behaviour
* Define HMI relevant behaviour e.g. SNA/ERR behaviour, decimal places to show, decimal separator symbol (, or .) , negative/positive sign visualization, position on the display etc
* Define state machine

1. Speedometer
* Speedometer will be comprised of Digital Value, Unit and Curved Scale
* Speedometer indication will be positioned with top margin 64px and left margin 16px
* Speedometer Range shall be 0 to 260 km/h
* Speedometer unit shall be km/h (coded by some EEPROM parameter)
* 
2. Odometer
* The Odometer shall be displayed centered with bottom margin 8px, 32px font for numerical value, 16px font for unit value.
* The dipslayed Odometer Range shall be 0 to 999 999 km
* The Resolution of the Odometer will be 1 km
* Leading zeros will be displayed till the fourth digits (thousands) if Odometer value is less than or equal to 999km
* Invalid values, SNA and Value not received will be displayed as dashes ----
3. Temperature
* The Teperature will be positioned centered horizontally with top margin 8px, with font size 32 px
* The dispalyed temp range shall be -40 to 80°C
* Values will be displayed with a resolution of 0.1°
* Values will be displayed without leading zeros
* Values smaller than zero will be displayed with a preceding negative sign
* Values greater than or equal to zero will be dispayed without a positive sign
* Invalid values, SNA and Value not received will be displayed as dashes --
* Damping??
4. Blinkers (Graphic only or with Acoustic???)
