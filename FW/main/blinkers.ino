#ifdef BLINKERS

#define buttonLeft  5  // Pin for left blink button
#define buttonRight 6 // Pin for right blink button
#define BS_FEATURE_CYCLE_TIME 50

#define PRESSED 0
#define RELEASED 1

enum blinker_state {BS_INIT, BS_IDLE, BS_BLINKING, BS_PREPARE_TO_IDLE, BS_PREPARE_TO_SWITCH, BS_FAIL} bs_stateVariable;
enum blinker_sub_state {BSS_OFF, BSS_LEFT, BSS_RIGHT, BSS_WARNING, BSS_FAIL} bs_substateVariable;
enum blinker_event {BE_NONE, BE_LEFT_BNT_PRESS, BE_RIGHT_BTN_PRESS, BE_BOTH_BTN_PRESS } be_eventVariable;

uint32_t bs_msCounts;
uint32_t bs_prevMillis;

int bl_left_btn_press;
int bl_right_btn_press;
int bl_left_btn_prev_state;
int bl_right_btn_prev_state;

void blinker_state_machine() {

  if (bs_prevMillis != millis()) {
    bs_msCounts++;
    bs_prevMillis = millis();
  }

  bl_left_btn_press = digitalRead(buttonLeft);
  if (bl_left_btn_press == PRESSED && bl_left_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
    if (be_eventVariable == BE_NONE)
      be_eventVariable = BE_LEFT_BNT_PRESS;
    if (be_eventVariable == BE_RIGHT_BTN_PRESS)
      be_eventVariable = BE_BOTH_BTN_PRESS;
  }
  bl_left_btn_prev_state = bl_left_btn_press;

  bl_right_btn_press = digitalRead(buttonRight);
  if (bl_right_btn_press == PRESSED && bl_right_btn_prev_state == RELEASED ) { // Button was pressed; TO-DO - debounce
    if (be_eventVariable == BE_NONE)
      be_eventVariable = BE_RIGHT_BNT_PRESS;
    if (be_eventVariable == BE_LEFT_BTN_PRESS)
      be_eventVariable = BE_BOTH_BTN_PRESS;
  }
  bl_right_btn_prev_state = bl_right_btn_press;


  if (bs_msCounts < BS_FEATURE_CYCLE_TIME) { // If there are no events pending, or cycle time hasn't expired --> exit
    return;
  }

  switch (be_eventVariable) {

    case BE_NONE:
      break;

    case BE_LEFT_BNT_PRESS:
      if (bs_stateVariable == BS_IDLE) {
        bs_stateVariable = BS_BLINKING;
        bs_substateVariable = BSS_LEFT;
      }

      if (bs_stateVariable == BS_BLINKING && bs_substateVariable == BSS_LEFT) {
        bs_stateVariable = BS_PREPARE_TO_IDLE;
      }

      if (bs_stateVariable == BS_BLINKING && bs_substateVariable == BSS_RIGHT) {
        bs_stateVariable = BS_PREPARE_TO_SWITCH;
      }
      break;


    case BE_RIGHT_BNT_PRESS:
      if (bs_stateVariable == BS_IDLE) {
        bs_stateVariable = BS_BLINKING;
        bs_substateVariable = BSS_RIGHT;
      }

      if (bs_stateVariable == BS_BLINKING && bs_substateVariable == BSS_RIGHT) {
        bs_stateVariable = BS_PREPARE_TO_IDLE;
      }

      if (bs_stateVariable == BS_BLINKING && bs_substateVariable == BSS_LEFT) {
        bs_stateVariable = BS_PREPARE_TO_SWITCH;
      }

      break;

    case BE_BOTH_BTN_PRESS:
      // TODO

      break

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
      blinker_sub_state = BSS_OFF;
      bs_msCounts = 0;
      break;


    case BS_IDLE:
      //do nothing
      break;

    case BS_BLINKING:  
    break;
    
  }
#endif
