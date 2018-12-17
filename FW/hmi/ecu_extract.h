//32 bytes PDU
struct PDU1_Struct {
  uint8_t pdu_id=1;
  uint8_t HMI_right_blinker_state;  // 0 = deactivated, 1 = active, 2 = RES, 3 = SNA
  uint8_t HMI_left_blinker_state;   // 0 = deactivated, 1 = active, 2 = RES, 3 = SNA
  uint16_t DispSpeed;
  uint8_t UnitSpeed;
  uint32_t DispDist;
  uint16_t MaxSpeed;
  int DispTemp;
  uint8_t UnitTemp;
  uint8_t padding[15];  // Reduce as needed!!!!!!!!!!!!!!
  uint8_t alive=0;
  uint8_t crc;
  
}PDU1_storage;
