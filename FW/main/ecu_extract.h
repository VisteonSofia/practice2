
//32 bytes PDU
struct PDU1_Struct {
  uint8_t HMI_right_blinker_state;  // 0 = deactivated, 1 = active, 2 = RES, 3 = SNA
  uint8_t HMI_left_blinker_state;   // 0 = deactivated, 1 = active, 2 = RES, 3 = SNA
  uint8_t DispSpeed;
  uint8_t UnitSpeed;
  uint8_t padding[29];  // Reduce as needed!!!!!!!!!!!!!!
  
} PDU1_storage;
