 
  #ifdef BLINKERS
  
//  #define BL_DEBUG 1 //You will need SERIAL in dummies_master
  #define buttonLeft  6  // Pin for left blink button
  #define buttonRight 5 // Pin for right blink button
  #define buttonHazzards 3 // Pin for Hazzards blink button
  
  #define BS_FEATURE_CYCLE_TIME 50
  #define BS_ACTIVE_TIME 600
  #define BS_WAITING_TIME 1000

  enum blinker_state {BS_INIT, BS_IDLE, BS_LEFT, BS_RIGHT, BS_HAZZARDS, BS_WAIT, BS_FAIL} bs_stateVariable;
  enum blinker_event {BE_NONE, BE_LEFT_BTN_PRESS, BE_RIGHT_BTN_PRESS, BE_HAZZARDS_BTN_PRESS , BE_FAIL} be_eventVariable;

  uint32_t bs_msCounts;
  uint32_t bs_prevMillis;
  
  int bl_left_btn_press;
  int bl_left_btn_prev_state;
  int bl_right_btn_press;
  int bl_right_btn_prev_state;
  int bl_hazzards_btn_press;
  int bl_hazzards_btn_prev_state;
  
  int active_time;
  int waiting_time;
  void blinkers_init(){
    pinMode(buttonLeft, INPUT_PULLUP);  // Initialize left button pin as INPUT with internal pullup enabled. This means we can skip the external pullup
    pinMode(buttonRight, INPUT_PULLUP);
    pinMode(buttonHazzards, INPUT_PULLUP);
  
    bl_left_btn_press = RELEASED;
    bl_right_btn_press = RELEASED;
    bl_left_btn_prev_state = RELEASED;
    bl_right_btn_prev_state = RELEASED;
    bl_hazzards_btn_press = RELEASED;
    bl_hazzards_btn_prev_state = RELEASED;
  
    be_eventVariable = BE_NONE;
    bs_stateVariable = BS_IDLE;
    bs_msCounts = 0;
    waiting_time = 0;

  }
  void blinker_state_machine() {
  
    if (bs_prevMillis != millis()) {
      bs_msCounts += (millis() - bs_prevMillis);
      if (active_time > 0) {
        active_time -= (millis() - bs_prevMillis);
      }
      if (waiting_time > 0) {
        waiting_time -= (millis() - bs_prevMillis);
      }
      bs_prevMillis = millis();
    }
  
    bl_left_btn_press = digitalRead(buttonLeft);
    bl_right_btn_press = digitalRead(buttonRight);
  
  
  
    // both blinkers button are not pressed
    if (!(bl_left_btn_press == PRESSED && bl_left_btn_prev_state == RELEASED && bl_right_btn_press == PRESSED && bl_right_btn_prev_state == RELEASED)) {
      // needs refacturing - do it once
  
      if (bl_left_btn_press == PRESSED && bl_left_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
        
  #ifdef BL_DEBUG
        Serial.print("Left button state:"); Serial.print(bl_left_btn_press); Serial.print("  | Right button state:"); Serial.println(bl_right_btn_press);
        Serial.println("Yay!!! left button pressed!!");
  #endif
  
        if (be_eventVariable == BE_LEFT_BTN_PRESS) {
          be_eventVariable = BE_NONE;
          bs_msCounts = 0;
        }
        else if (be_eventVariable != BE_HAZZARDS_BTN_PRESS) {
          be_eventVariable = BE_LEFT_BTN_PRESS;
          bs_msCounts = 0;
        }
  
      }
      bl_left_btn_prev_state = bl_left_btn_press;
  
      if (bl_right_btn_press == PRESSED && bl_right_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
  #ifdef BL_DEBUG
        Serial.println("Yay!!! right button pressed!!");
  #endif
        if (be_eventVariable == BE_RIGHT_BTN_PRESS) {
          be_eventVariable = BE_NONE;
          bs_msCounts = 0;
        }
        else if (be_eventVariable != BE_HAZZARDS_BTN_PRESS) {
          be_eventVariable = BE_RIGHT_BTN_PRESS;
          bs_msCounts = 0;
        }
      }
      bl_right_btn_prev_state = bl_right_btn_press;
    } // Rising front on both buttons?
  
    bl_hazzards_btn_press = digitalRead(buttonHazzards);
    if (bl_hazzards_btn_press == PRESSED && bl_hazzards_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
  #ifdef BL_DEBUG
      Serial.println("Yay!!! hazzard button pressed!!");
  #endif
      if (be_eventVariable == BE_HAZZARDS_BTN_PRESS) {
        be_eventVariable = BE_NONE;
        bs_msCounts = 0;
      }
      else {
        be_eventVariable = BE_HAZZARDS_BTN_PRESS;
        bs_msCounts = 0;
      }
    }
    bl_hazzards_btn_prev_state = bl_hazzards_btn_press;
  
    if (bl_left_btn_press != PRESSED && bl_left_btn_press != RELEASED) {
      be_eventVariable = BE_FAIL;
    }
    if (bl_right_btn_press != PRESSED && bl_right_btn_press != RELEASED) {
      be_eventVariable = BE_FAIL;
    }
    if (bl_hazzards_btn_press != PRESSED && bl_hazzards_btn_press != RELEASED) {
      be_eventVariable = BE_FAIL;
    }
  #ifdef BL_DEBUG
    Serial.print("Time"); Serial.print(bs_msCounts); Serial.print(" | Event "); Serial.println(be_eventVariable);
    Serial.print("bs_stateVariable"); Serial.println(bs_stateVariable);
    Serial.print("active_time"); Serial.print(active_time); Serial.print(" | waiting_time "); Serial.println(waiting_time);
  #endif
    if (bs_msCounts < BS_FEATURE_CYCLE_TIME || active_time > 0) { // If there are no events pending, or cycle time hasn't expired --> exit
      return;
    }
  
    if (waiting_time <= 0) {
      switch (be_eventVariable) {
  
        case BE_NONE:
          bs_stateVariable == BS_IDLE;
          break;
  
        case BE_LEFT_BTN_PRESS:
          bs_stateVariable = BS_LEFT;
          break;
  
        case BE_RIGHT_BTN_PRESS:
          bs_stateVariable = BS_RIGHT;
          break;
  
        case BE_HAZZARDS_BTN_PRESS:
          bs_stateVariable = BS_HAZZARDS;
          break;
        case BE_FAIL:
          bs_stateVariable = BS_FAIL;
          break;
        default:
          break;
      }
    }
    else {
      if(bs_stateVariable != BS_WAIT) 
          EasyBuzzer.singleBeep(500,30);
      bs_stateVariable = BS_WAIT;
    }
  #ifdef BL_DEBUG
    Serial.print("State"); Serial.println(bs_stateVariable);
  #endif
  
    switch (bs_stateVariable) {
  
      case BS_INIT:
  
        bl_left_btn_press = RELEASED;
        bl_right_btn_press = RELEASED;
        bl_left_btn_prev_state = RELEASED;
        bl_right_btn_prev_state = RELEASED;
        bl_hazzards_btn_press = RELEASED;
        bl_hazzards_btn_prev_state = RELEASED;
  
        be_eventVariable = BE_NONE;
        bs_stateVariable = BS_IDLE;
        bs_msCounts = 0;
        waiting_time = 0;
        break;
  
      case BS_IDLE:
  //      digitalWrite(leftBlinkerLED, LOW);
          PDU1_storage.HMI_left_blinker_state = 0;
  //      digitalWrite(rightBlinkerLED, LOW);
          PDU1_storage.HMI_right_blinker_state = 0;
        break;
  
      case BS_LEFT:
//        digitalWrite(leftBlinkerLED, HIGH);
        PDU1_storage.HMI_left_blinker_state = 1;
//        digitalWrite(rightBlinkerLED, LOW);
        PDU1_storage.HMI_right_blinker_state = 0;
        EasyBuzzer.singleBeep(700,40);
         
        active_time = BS_ACTIVE_TIME;
        waiting_time = BS_WAITING_TIME;
        break;
  
      case BS_RIGHT:
 //       digitalWrite(leftBlinkerLED, LOW);
        PDU1_storage.HMI_left_blinker_state = 0;
 //       digitalWrite(rightBlinkerLED, HIGH);
        PDU1_storage.HMI_right_blinker_state = 1;
        EasyBuzzer.singleBeep(700,40);
                
        active_time = BS_ACTIVE_TIME;
        waiting_time = BS_WAITING_TIME;
        break;
  
      case BS_HAZZARDS:
 //       digitalWrite(leftBlinkerLED, HIGH);
        PDU1_storage.HMI_left_blinker_state = 1;
 //       digitalWrite(rightBlinkerLED, HIGH);
        PDU1_storage.HMI_right_blinker_state = 1;
        EasyBuzzer.singleBeep(700,40);
                
        active_time = BS_ACTIVE_TIME;
        waiting_time = BS_WAITING_TIME;
        break;
  
      case BS_WAIT:
//        digitalWrite(leftBlinkerLED, LOW);
        PDU1_storage.HMI_left_blinker_state = 0;
//        digitalWrite(rightBlinkerLED, LOW);
        PDU1_storage.HMI_right_blinker_state = 0;
        break;
  
      case BS_FAIL:
//        digitalWrite(leftBlinkerLED, HIGH);
        PDU1_storage.HMI_left_blinker_state = 1;
//        digitalWrite(rightBlinkerLED, HIGH);
        PDU1_storage.HMI_right_blinker_state = 1;
        break;
      default:
        break;

  }
}
#endif
