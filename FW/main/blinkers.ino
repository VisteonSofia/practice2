#ifdef BLINKERS

#define buttonLeft  5  // Pin for left blink button
#define buttonRight 6 // Pin for right blink button
#define buttonHazzards 7 // Pin for Hazzards blink button
#define leftBlinkerLED 8 
#define rightBlinkerLED 9
#define BS_FEATURE_CYCLE_TIME 50
#define BS_ACTIVE_TIME 600
#define BS_WAITING_TIME 400

#define PRESSED 0
#define RELEASED 1

enum blinker_state {BS_INIT, BS_IDLE, BS_LEFT, BS_RIGHT, BS_HAZZARDS, BS_WAIT, BS_FAIL} bs_stateVariable;
enum blinker__event {BE_NONE,BE_LEFT_BNT_PRESS, BE_RIGHT_BTN_PRESS, BE_HAZZARDS_BTN_PRESS , BE_FAIL} be_eventVariable;

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


void blinker_state_machine() {

  if (bs_prevMillis != millis()) {
    bs_msCounts++;
    if(active_time>0){
      active_time--;
    }
    if(waiting_time>0){
      waiting_time--;
    }
    bs_prevMillis = millis();
  }
  // both blinkers button are not pressed
  if(bl_left_btn_press != PRESSED&&bl_left_btn_prev_state == RELEASED&&bl_right_btn_press != PRESSED&&bl_right_btn_prev_state == RELEASED){
  // needs refacturing - do it once
    bl_left_btn_press = digitalRead(buttonLeft);
    if (bl_left_btn_press == PRESSED && bl_left_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
      if(be_eventVariable == BE_LEFT_BNT_PRESS){
        be_eventVariable=BE_NONE;
        bs_msCounts=0;
      }
      else if(be_eventVariable != BE_HAZZARDS_BNT_PRESS){
        be_eventVariable=BE_LEFT;
        bs_msCounts=0;
      }
      
    }
    bl_left_btn_prev_state = bl_left_btn_press;
    
    bl_right_btn_press = digitalRead(buttonRight);
    if (bl_right_btn_press == PRESSED && bl_right_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
      if(be_eventVariable == BE_RIGHT_BNT_PRESS){
        be_eventVariable=BE_NONE;
        bs_msCounts=0;
      }
      else if(be_eventVariable != BE_HAZZARDS_BNT_PRESS){
        be_eventVariable=BE_RIGHT;
        bs_msCounts=0;
      }
    }
    bl_right_btn_prev_state = bl_right_btn_press;
  }
  
  bl_hazzards_btn_press = digitalRead(buttonHazzards);
  if (bl_hazzards_btn_press == PRESSED && bl_hazzards_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
      if(be_eventVariable == BE_HAZZARDS_BNT_PRESS){
      be_eventVariable=BE_NONE;
      bs_msCounts=0;
    }
    else {
      be_eventVariable=BE_HAZZARDS_BNT_PRESS;
      bs_msCounts=0;
    }
  }
  bl_hazzards_btn_prev_state = bl_hazzards_btn_press;
  
  if(bl_left_btn_press != PRESSED && bl_left_btn_press != RELEASED){
    be_eventVariable=BE_FAIL;
  }
  if(bl_right_btn_press != PRESSED && bl_right_btn_press != RELEASED){
    be_eventVariable=BE_FAIL;
  }
  if(bl_hazzards_btn_press != PRESSED && bl_hazzards_btn_press != RELEASED){
    be_eventVariable=BE_FAIL;
  }
  
  if (bs_msCounts != BS_FEATURE_CYCLE_TIME||active_time>0||waiting_time>0) { // If there are no events pending, or cycle time hasn't expired --> exit
    return;
  }
  
  
  switch (be_eventVariable) {

    case BE_NONE:
      bs_stateVariable == BS_IDLE;
      break;

    case BE_LEFT_BNT_PRESS:
      if (bs_stateVariable == BS_LEFT) {
        bs_stateVariable = BS_WAIT;
      }
      else{
        bs_stateVariable = BS_LEFT;
      }
      break;
      
    case BE_RIGHT_BNT_PRESS:
      if (bs_stateVariable == BS_RIGHT) {
        bs_stateVariable = BS_WAIT;
      }
      else{
        bs_stateVariable = BS_RIGHT;
      }
      break;

    case BE_HAZZARDS_BTN_PRESS:
     if (bs_stateVariable == BS_HAZZARDS) {
        bs_stateVariable = BS_WAIT;
      }
      else{
        bs_stateVariable = BS_HAZZARDS;
      }
      break;
    case BE_FAIL:
      bs_stateVariable = BS_FAIL;
      break;
    default:
      break;
  }


  switch (bs_stateVariable) {

    case BS_INIT:
      pinMode(buttonLeft, INPUT_PULLUP);  // Initialize left button pin as INPUT with internal pullup enabled. This means we can skip the external pullup
      pinMode(buttonRight, INPUT_PULLUP);

      bl_left_btn_press = FALSE;
      bl_right_btn_press = FALSE;
      bl_left_btn_prev_state = FALSE;
      bl_right_btn_prev_state = FALSE;

      be_eventVariable = BE_NONE;
      bs_stateVariable = BS_IDLE;
      bs_msCounts = 0;
      waiting_time=0;
      break;


    case BS_IDLE:
      digitalWrite(leftBlinkerLED, LOW);
      digitalWrite(rightBlinkerLED, LOW);
      break;
      
    case BS_LEFT:
      digitalWrite(leftBlinkerLED, HIGH);
      digitalWrite(rightBlinkerLED, LOW);
      active_time=BS_ACTIVE_TIME;
      break;
      
    case BS_RIGHT:
      digitalWrite(leftBlinkerLED, LOW);
      digitalWrite(rightBlinkerLED, HIGH);
      active_time=BS_ACTIVE_TIME;
      break;
    
    case BS_HAZZARDS:
      digitalWrite(leftBlinkerLED, HIGH);
      digitalWrite(rightBlinkerLED, HIGH);
      active_time=BS_ACTIVE_TIME;
      break;
      
    case BS_WAIT:
      digitalWrite(leftBlinkerLED, LOW);
      digitalWrite(rightBlinkerLED, LOW);
      waiting_time=BS_WAITING_TIME;
      break;
      
    case BS_FAIL:
      digitalWrite(leftBlinkerLED, HIGH);
      digitalWrite(rightBlinkerLED, HIGH);
      break;
    default:
      break;
    
  }
#endif
