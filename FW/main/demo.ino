#ifdef DEMO
  
// #define DEMO_DEBUG
#define DE_STARTING 1000 //0.3sec
#define DE_IDLE_TIME 300 //0.3sec
#define indent_temp 34

enum de_state {DE_INIT, DE_IDLE, DE_TRANSMIT} de_stateVariable;
uint32_t de_msCounts=0,de_prevMillis=0;
uint16_t indent_speed;
bool is_up=true;
uint8_t demo_blinkers=0;

void de_state_machine() 
{
  
  if (de_prevMillis != millis()) {
    de_msCounts += (millis() - de_prevMillis);
        
    de_prevMillis = millis();
  }
  

  switch (de_stateVariable) {
    case DE_INIT:
      if(de_msCounts>=DE_STARTING){
        de_stateVariable=DE_IDLE;
        de_msCounts=0;
      }
      indent_speed=storage.mvs_spd_max_speed/60;
      PDU1_storage.MaxSpeed= storage.mvs_spd_max_speed;
    break;

    case DE_IDLE:
        
      if(de_msCounts >= DE_IDLE_TIME)
      {
        de_stateVariable = DE_TRANSMIT;
        de_msCounts = 0;
      }
    break; 

    case DE_TRANSMIT:
      if(PDU1_storage.DispSpeed+indent_speed<storage.mvs_spd_max_speed&&is_up){
        PDU1_storage.DispSpeed+=indent_speed;
      }
      else if(is_up){
        PDU1_storage.DispSpeed=storage.mvs_spd_max_speed;
        is_up=false;
      }
      else if(PDU1_storage.DispSpeed>indent_speed&&!is_up){
        PDU1_storage.DispSpeed-=indent_speed;
      }
      else{
         PDU1_storage.DispSpeed=0;
         PDU1_storage.UnitSpeed=!PDU1_storage.UnitSpeed;
         PDU1_storage.UnitTemp=!PDU1_storage.UnitTemp;
         storage.mvs_cycles_in_demo--;
         saveConfig();
         PDU1_storage.DispTemp=-550;
         PDU1_storage.DispDist= 0;
         is_up=true;
      }
      
      if(PDU1_storage.DispSpeed<storage.mvs_spd_max_speed/4||demo_blinkers>2){
          PDU1_storage.HMI_left_blinker_state=0;
          PDU1_storage.HMI_right_blinker_state=0;
          demo_blinkers=0;
      }
      else if(PDU1_storage.DispSpeed<storage.mvs_spd_max_speed/2){
          PDU1_storage.HMI_left_blinker_state=1;
          PDU1_storage.HMI_right_blinker_state=0;
          demo_blinkers++;
      }
      else if(PDU1_storage.DispSpeed<storage.mvs_spd_max_speed*3/4){
          PDU1_storage.HMI_left_blinker_state=0;
          PDU1_storage.HMI_right_blinker_state=1;
          demo_blinkers++;
      }
      else{
          PDU1_storage.HMI_left_blinker_state=1;
          PDU1_storage.HMI_right_blinker_state=1;
          demo_blinkers++;
      }
      PDU1_storage.DispDist+=10;
      if(PDU1_storage.DispTemp+indent_temp<1448&&is_up){
        PDU1_storage.DispTemp+=indent_temp;
      }
      else if(is_up){
        PDU1_storage.DispTemp=1448;
      }
      else if(PDU1_storage.DispTemp-indent_temp>-550&&!is_up){
        PDU1_storage.DispTemp-=indent_temp;
      }
      else{
         PDU1_storage.DispTemp=-550;
      }
      de_stateVariable = DE_IDLE;
        de_msCounts = 0;
    break;
  }
}
#endif
