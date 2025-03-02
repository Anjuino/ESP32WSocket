#include "MyCommon.h"
#include "Settings.h"
#include "PacketBIN.h"
#include "PacketString.h"
#include "MySocket.h"



void setup() {
  Serial.begin(9600);
  EEPROM.begin(128);
  GetSettings();
  WifiInit();
  WebSocketInit();

  #ifdef BME280Sensor
    bme.begin(0x76);
  #endif

  #ifdef CONTROLLER_LED
    Ws2812Init();
    Serial.println("Создаю задачу для ленты");
    Serial.println(CountLed);
    xTaskCreatePinnedToCore (
      TaskLed, 
      "Task1",      
      50000,        
      NULL,         
      1,            
      &Task1,       
      1);                            
    delay(500);

    #ifdef DETECTED_SENSOR
      xTaskCreatePinnedToCore (
        TaskDetectedf, 
        "Task2",      
        4096,        
        NULL,         
        1,            
        &TaskDetected,       
        1); 
    #endif
  #endif
}

#ifdef CONTROLLER_LED
  void TaskLed(void * pvParameters){
    for(;;) {
      Ws2812Loop ();
      delay (0);
    } 
  }
#endif

#ifdef CONTROLLER_LED
  #ifdef DETECTED_SENSOR
    void TaskDetectedf (void * pvParameters ) {
      bool IsWork   = false;   // Флаг работы системы
      uint32_t TimerWork;
      for(;;) {
        if (Automode) {       
          if (IsDetectedMove) {                           // это по прерыванию с датчика движения
            IsDetectedMove = false;
            if (ReadLight() < 300 || IsWork) {            // тут уровень освещения и если система уже сработала
              if (!IsWork) Ws2812SetMode(249);
              IsWork = true;
              TimerWork = millis() + 180000;            // Обновил время
            }           
          }

          if (IsWork) {
            if (millis () > TimerWork) {
              IsWork = false;
              TimerWork = 0;
              Ws2812SetMode(250);
            }
          }
        }
      }
    }
  #endif
#endif

void loop() {

  webSocket.loop();

  #ifdef CONTROLLER_TELEMETRY
    #ifdef TEMPERATURE_SENSOR
      if (millis() > Timer1) {
        Timer1 = millis() + TimerTempAndHum;
        SendPacketTepmHum();
      }
    #endif  

    #ifdef CO2_SENSOR
      if (millis() > Timer2) {
        Timer2 = millis() + TimerCO2;
        SendPacketCO2();
      }

      #ifdef MQ135Sensor
        if (millis() > TimerCallibrate) {
          TimerCallibrate = millis() + 60000;
          CallibrateMQ135();
        }
      #endif
    #endif

    #ifdef PRESSURE_SENSOR
      if (millis() > Timer3) {
        Timer3 = millis() + TimerPressure;
        SendPacketPressure();
      }
    #endif
  #endif
}