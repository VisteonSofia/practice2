#ifdef BLINKERS

#define BL_DEBUG 1

#define BS_FEATURE_CYCLE_TIME 50
#define BS_ACTIVE_TIME 600 
#define BS_WAITING_TIME 400




void blinker_state_machine() {

  if (bs_prevMillis != millis()) {
    
    bs_msCounts+= (millis()-bs_prevMillis);
    
    if (active_time > 0) {
      active_time-=(millis()-bs_prevMillis);
    }
    if (waiting_time > 0) {
      waiting_time-=(millis()-bs_prevMillis);
    }
    bs_prevMillis = millis();
  }

  bl_left_btn_press = digitalRead(buttonLeft);
  bl_right_btn_press = digitalRead(buttonRight);



  // both blinkers button are not pressed
 if(!(bl_left_btn_press == PRESSED && bl_left_btn_prev_state == RELEASED && bl_right_btn_press == PRESSED && bl_right_btn_prev_state == RELEASED)){
    // needs refacturing - do it once

    if (bl_left_btn_press == PRESSED && bl_left_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
    Serial.print("Left button state:"); Serial.print(bl_left_btn_press); Serial.print("  | Right button state:"); Serial.println(bl_right_btn_press);
#ifdef BL_DEBUG
      Serial.println("Yay!!! left button pressed!!");
#endif BL_DEBUG

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
#endif BL_DEBUG
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
#endif BL_DEBUG
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
    Serial.print("Time");Serial.print(bs_msCounts);Serial.print(" | Event ");Serial.println(be_eventVariable);
    Serial.print("bs_stateVariable");Serial.println(bs_stateVariable);
    Serial.print("active_time");Serial.print(active_time);Serial.print(" | waiting_time ");Serial.println(waiting_time);
#endif BL_DEBUG
  if (bs_msCounts < BS_FEATURE_CYCLE_TIME || active_time > 0) { // If there are no events pending, or cycle time hasn't expired --> exit
    return;
  }
  if(waiting_time <= 0){
    switch (be_eventVariable) {

      case BE_NONE:
        bs_stateVariable == BS_IDLE;
        break;

      case BE_LEFT_BTN_PRESS:
        if (bs_stateVariable == BS_LEFT) {
          bs_stateVariable = BS_WAIT;
        }
        else {
          bs_stateVariable = BS_LEFT;
        }
        break;

      case BE_RIGHT_BTN_PRESS:
        if (bs_stateVariable == BS_RIGHT) {
          bs_stateVariable = BS_WAIT;
        }
        else {
          bs_stateVariable = BS_RIGHT;
        }
        break;

      case BE_HAZZARDS_BTN_PRESS:
        if (bs_stateVariable == BS_HAZZARDS) {
          bs_stateVariable = BS_WAIT;
        }
        else {
          bs_stateVariable = BS_HAZZARDS;
        }
        break;
      case BE_FAIL:
        bs_stateVariable = BS_FAIL;
        break;
      default:
        break;
    }
  }
#ifdef BL_DEBUG
    Serial.print("State");Serial.println(bs_stateVariable);
#endif BL_DEBUG

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
      digitalWrite(leftBlinkerLED, LOW);
      digitalWrite(rightBlinkerLED, LOW);
      break;

    case BS_LEFT:
      digitalWrite(leftBlinkerLED, HIGH);
      digitalWrite(rightBlinkerLED, LOW);
      active_time = BS_ACTIVE_TIME;
      break;

    case BS_RIGHT:
      digitalWrite(leftBlinkerLED, LOW);
      digitalWrite(rightBlinkerLED, HIGH);
      active_time = BS_ACTIVE_TIME;
      break;

    case BS_HAZZARDS:
      digitalWrite(leftBlinkerLED, HIGH);
      digitalWrite(rightBlinkerLED, HIGH);
      active_time = BS_ACTIVE_TIME;
      break;

    case BS_WAIT:
      digitalWrite(leftBlinkerLED, LOW);
      digitalWrite(rightBlinkerLED, LOW);
      waiting_time = BS_WAITING_TIME;
      break;

    case BS_FAIL:
      digitalWrite(leftBlinkerLED, HIGH);
      digitalWrite(rightBlinkerLED, HIGH);
      break;
    default:
      break;

  }
}
#endif
