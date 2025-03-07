#include <EEPROM.h>

#define SETTINGSADDRES 0

////////////////// Структура с настройками устройства хранимых в EEPROM //////////////////////////
struct DeviceSettings {
  uint8_t UID = 0;

  #ifdef CONTROLLER_TELEMETRY
    #ifdef TEMPERATURE_SENSOR
      uint32_t TimerTempAndHum;
      bool AlertTempAndHumIsOn;
      int16_t MaxLimitT;
      int16_t MinLimitT;
    #endif

    #ifdef CO2_SENSOR
      uint32_t TimerCO2;
      bool AlertCO2IsOn;
      int16_t MaxLimitCO2;
    #endif

    #ifdef PRESSURE_SENSOR
      uint32_t TimerPressure;
      bool AlertPressureIsOn;
    #endif
  #endif

  #ifdef CONTROLLER_LED
    uint32_t CountLed;
    #ifdef DETECTED_SENSOR
      uint8_t ModeWork;
    #endif
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
  bool NeedUpdate = false;

  #ifdef CONTROLLER_TELEMETRY
    #ifdef TEMPERATURE_SENSOR
      TimerTempAndHum = Settings.TimerTempAndHum;
      if (TimerTempAndHum   == 4294967295 || TimerTempAndHum == 0) {
        TimerTempAndHum   = 60000;
        Settings.TimerTempAndHum = 60000;
        Serial.println("Вызвал перезапись ВРЕМЯ Т");
        NeedUpdate = true;
      }

      TempAndHumAlert = Settings.AlertTempAndHumIsOn;

      MaxLimitT = Settings.MaxLimitT;
      if (MaxLimitT == 32767 || MaxLimitT == -1) {
        MaxLimitT = 30;
        Settings.MaxLimitT = MaxLimitT;
        NeedUpdate = true;
        Serial.println("Вызвал перезапись МАКС Т");
      }

      MinLimitT = Settings.MinLimitT;
      if (MinLimitT == 32767 || MinLimitT == -1) {
        MinLimitT = 10;
        Settings.MinLimitT = MinLimitT;
        NeedUpdate = true;
        Serial.println("Вызвал перезапись МИН Т");
      }
    #endif 

    #ifdef CO2_SENSOR
      TimerCO2 = Settings.TimerCO2;
      if (TimerCO2 == 4294967295 || TimerCO2 == 0) {
        TimerCO2 = 600000;
        Settings.TimerCO2 = TimerCO2;
        NeedUpdate = true;
        Serial.println("Вызвал перезапись ВРЕМЯ СО2");
      }

      CO2Alert = Settings.AlertCO2IsOn;

      MaxLimitCO2 = Settings.MaxLimitCO2;
      if (MaxLimitCO2 == 32767 || MaxLimitCO2 == -1) {
        MaxLimitCO2 = 900;
        Settings.MaxLimitCO2 = MaxLimitCO2;
        NeedUpdate = true;
        Serial.println("Вызвал перезапись МАКС СО2");
      }
    #endif

    #ifdef PRESSURE_SENSOR
      TimerPressure = Settings.TimerPressure;
      if (TimerPressure == 4294967295 || TimerPressure == 0) {
        TimerPressure = 600000;
        Settings.TimerPressure = TimerPressure;
        NeedUpdate = true;
      }
      
      PressureAlert = Settings.AlertPressureIsOn;
    #endif  
  #endif

  #ifdef CONTROLLER_LED
    CountLed = Settings.CountLed;
    Serial.println(CountLed);
    if (CountLed > 1000) {
      CountLed = 50;
      Settings.CountLed = CountLed;
      NeedUpdate = true;
    }
    strip.updateLength (CountLed);

    #ifdef DETECTED_SENSOR
      Automode = Settings.ModeWork;
      Serial.println(Automode);
    #endif
  #endif

  if (NeedUpdate) {
    Serial.println("Перезаписываю настройкиииииии");
    WriteSettings ();
  }  
}
