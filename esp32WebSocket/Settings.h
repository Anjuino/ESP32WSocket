#include <EEPROM.h>

#define SETTINGSADDRES 0

////////////////// Структура с настройками устройства хранимых в EEPROM //////////////////////////
struct DeviceSettings {
  uint8_t UID = 0;
};

DeviceSettings Settings;
////////////////////////////////////////////////////////////////////////////////


void GetSettings ()
{ 
  EEPROM.get(SETTINGSADDRES, Settings);
  uint8_t UID = Settings.UID;
  if (UID == 255) Settings.UID = 0;
}

void WriteSettings ()
{
  EEPROM.put(SETTINGSADDRES, Settings);
  EEPROM.commit();
  EEPROM.end();  
}