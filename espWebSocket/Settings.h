#include <EEPROM.h>

#define SETTINGSADDRES 0

////////////////// Структура с настройками устройства хранимых в EEPROM //////////////////////////
struct DeviceSettings {
  uint8_t UID = 0;

  #ifdef CONTROLLER_TELEMETRY
    #ifdef TEMPERATURE_SENSOR
      uint32_t TimerTempAndHum;
    #endif

    #ifdef CO2_SENSOR
      uint32_t TimerCO2;
    #endif
  #endif

  #ifdef CONTROLLER_LED
    //// ТУТ ДОБАВИТЬ ПОЛЯ ДЛЯ КАК В ПРОЕКТЕ WebLed
  #endif  
};

DeviceSettings Settings;
////////////////////////////////////////////////////////////////////////////////
void WriteSettings ()
{
  EEPROM.put(SETTINGSADDRES, Settings);
  EEPROM.commit();
  //EEPROM.end();  
}

void GetSettings ()
{ 
  EEPROM.get(SETTINGSADDRES, Settings);
  
  uint8_t UID =   Settings.UID;
  if (UID == 255) Settings.UID = 0;

  #ifdef CONTROLLER_TELEMETRY
  bool NeedUpdate = false;
    #ifdef TEMPERATURE_SENSOR
      TimerTempAndHum = Settings.TimerTempAndHum;
      if (TimerTempAndHum   == 4294967295 || TimerTempAndHum == 0) {
        TimerTempAndHum   = 60000;
        Settings.TimerTempAndHum = 60000;
        NeedUpdate = true;
      }
    #endif 

    #ifdef CO2_SENSOR
      TimerCO2 = Settings.TimerCO2;
      if (TimerCO2 == 4294967295 || TimerCO2 == 0) {
        TimerCO2 = 600000;
        Settings.TimerCO2 = TimerCO2;
        NeedUpdate = true;
      }
    #endif  

    if (NeedUpdate = true) {
      Serial.println("Перезаписываю настройки");
      WriteSettings ();
    }
  #endif  
}
