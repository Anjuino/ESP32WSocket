#include <EEPROM.h>

#define SETTINGSADDRES 0

////////////////// Структура с настройками устройства хранимых в EEPROM //////////////////////////
struct DeviceSettings {
  uint8_t UID = 0;
  #ifdef CONTROLLER_TELEMETRY
    uint32_t TimerDHT;
    uint32_t TimerMQ135;
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
  EEPROM.end();  
}

void GetSettings ()
{ 
  EEPROM.get(SETTINGSADDRES, Settings);
  
  uint8_t UID =   Settings.UID;
  if (UID == 255) Settings.UID = 0;

  #ifdef CONTROLLER_TELEMETRY
    TimerDHT            = Settings.TimerDHT;
    TimerMQ135          = Settings.TimerMQ135;

    bool NeedUpdate = false;
    if (TimerDHT   == 4294967295) {
      TimerDHT   = 60000;
      NeedUpdate = true;
    } 
    if (TimerMQ135 == 4294967295) {
      TimerMQ135 = 600000;
      NeedUpdate = true;
    }

    if (NeedUpdate = true) WriteSettings ();
  #endif  
}
