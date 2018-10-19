struct PDU1_Struct {
  uint16_t AmbientTemp; 
  uint8_t  DispSpeed;  // (0-160, step 1; 255 = SNA)
  uint32_t DispDist;
  uint16_t HMI_right_blinker_state;  // 0 = deactivated, 1 = active, 2 = RES, 3 = SNA
  uint16_t HMI_left_blinker_state;   // 0 = deactivated, 1 = active, 2 = RES, 3 = SNA
  uint8_t padding[21];  // Reduce as needed!!!!!!!!!!!!!!
} PDU1_storage;
