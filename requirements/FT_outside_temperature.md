Ambient temperature
	/revision 1/

1.  Ambient Temperature. Overall Description  

The Practice2 device shall display the ambient temperature, received from an internal transmitter, over a monochrome display.

1.1. Inputs:

Int_to_Fct_Amb_Temp
	[TBD- additional synch with Martin, for the external device data format]
	
	Valid values range: 0 .. 1998 (Phys. Range:  -55°C .. +144.8°C)
	Increment factor:  0.1°C
	Not valid values:
	  Ndef values: 1999 .. 65534
	  SNA: 65535
	
	1.2. Outputs:
Fct_to_HIM_ Amb_Temp
	[TBD]
	Valid values range: 0 .. 1998
	Phys. Range:  Phys. Range:  -55°C .. +144.8°C)
	Increment factor:  0.1°C
	Error (---.-):65535
1.3 NVM Parameter
	NVM_Temp_unit
	Values:
	    0: Celsius (default) 
	    1: Fahrenheit	

2.  Specific Requirements  

This section contains all the software requirements at a level of detail sufficient to enable designers to design a system to satisfy those requirements, and testers to test that the system satisfies those requirements.

2.1 Functional requirements
AT1: 	The functional SW component Ambient Temperature shall emit the Fct_to_HIM_ Amb_Temp with cyclic rate of 5s.

AT2: 	The output signal Fct_to_HIM_ Amb_Temp shall be mapped to the input signal value Int_to_Fct_Amb_Temp (Fct_to_HIM_ Amb_Temp = Int_to_Fct_Amb_Temp) with no additional filtering or hysteresis.

AT3:	 If the input signal value is out of the valid values range (Int_to_Fct_Amb_Temp == SNA OR  Int_to_Fct_Amb_Temp ==Ndef), the output signal Fct_to_HIM_ Amb_Temp shall be set to its  Error value.

2.2 HMI requirements

AT4: 	The HMI layer shall display the converted value received over the interface signal Fct_to_HIM_ Amb_Temp over the monochrome display as a decimal fraction number, with one fractional digit after decimal point, via the sample [XXX.X]° [C/F], with no additional rounding.

AT11:	The HMI layer shall display the converted value received over the interface signal Fct_to_HIM_ Amb_Temp over the monochrome display with no leading zeros.

AT5: 	The Fct_to_HIM_ Amb_Temp interface shall be received in celsius and shall be converted before displaying bt the HMI, based on the configuration NVM parameter NVM_Temp_Unit.

AT6:	If [NVM_Temp_Unit== Celsius], the HMI shall display the recived value of Fct_to_HIM_ Amb_Temp with no convertion.

AT7: 	If [NVM_Temp_Unit== Fahrenheit], the HMI shall use the following formula for Fct_to_HIM_ Amb_Temp convertion: T(°F) = T(°C) × 9/5 + 32.

AT8: 	The HMI shall display the corresponding temp unit symbol [C/F], based on the configuration NVM parameter NVM_Temp_Unit.

AT9: 	The degrees symbol “ ° ” shall be static and  shall alway be displayed with the Ampent Temperature screen composition. 

AT10: In case of [Fct_to_HIM_ Amb_Temp == Error], the HMI shall display dashes for the temperature, followed by the currently selected temperature unit: [- - -.-]° [C/F].


State chart:
 practice2/requirements/outside_temp_draft_visio_statemachine.jpg 
