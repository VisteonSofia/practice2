  #include <EEPROM.h>

  // ID of the settings block
  #define CONFIG_VERSION 5
  
  // Tell it where to store your config data in EEPROM
  #define CONFIG_START 32
  
  // Example settings structure
  struct StoreStruct {
    // The variables of your settings
    uint16_t diag_version,mvs_spd_max_speed:9; //max_speed 512
    uint32_t mvs_odo_range:17;
    bool mvs_spd_is_km, mvs_temp_is_celsius, mvs_is_acoustic_on;
    uint8_t mvs_cycles_in_demo;
    uint8_t mvs_end;
//    char c;
//    long d;
//    float e[6];
  } storage = {
    CONFIG_VERSION,
    // The default values
    250, 
    0,
    true, true, true,0,
    255
 //   'c',
 //   10000,
 //   {4.5, 5.5, 7, 8.5, 10, 12}
  };

  void loadConfig() {
    // To make sure there are settings, and they are YOURS!
    // If nothing is found it will use the default settings.
    if (EEPROM.read(CONFIG_START) == CONFIG_VERSION)
      for (unsigned int t=0; t<sizeof(storage); t++)
        *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
  }
  
  void saveConfig() {
    for (unsigned int t=0; t<sizeof(storage); t++)
      EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
  }

  //  Serial.println(storage.mvs_spd_max_speed);
//  // [...]
//  int i = storage.mvs_spd_max_speed - 10;
//  // [...]
//  Serial.print("Internal value of i:");
//  Serial.println(i);
//  // [...]
//  storage.mvs_spd_max_speed = 300;
//    saveConfig();
//    loadConfig();
//  // [...]
//  Serial.print("Current value_2:");
//  Serial.println(storage.mvs_spd_max_speed);
//
//  storage.mvs_spd_max_speed = 230;
//    saveConfig();
//    loadConfig();
//  // [...]
//  Serial.print("Current value_3:");
//  Serial.println(storage.mvs_spd_max_speed);
