* IPC (inter processor communication) feature will provide communication channel between the two processors of the system, as defined in system architecture and software architecture

* The feature will send data frame from main processor 1 to HMI processor 2 (unidirectional)
* The feature will send the applicable data frame(s) every 50ms
* The feature will consist of two software components - one sender running on processor 1 and one receiver running on processor 2
* The feature will utilize hardware I2C on the host microcontroller
* The feature will use I2C address 0x08
* The feature shall support DEBUG mode build based on a #define
