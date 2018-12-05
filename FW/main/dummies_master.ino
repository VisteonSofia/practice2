#define IPC
#define BLINKERS

#define buttonLeft  5  // Pin for left blink button
#define buttonRight 6 // Pin for right blink button
#define buttonHazzards 7 // Pin for Hazzards blink button

#define PRESSED 0
#define RELEASED 1

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

void setup() {

  Serial.begin(115200);
  Serial.println("Starting..");
  Serial.flush();
  
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

void loop() {

  blinker_state_machine();
  ipc_state_machine();
  delay(10);

}
