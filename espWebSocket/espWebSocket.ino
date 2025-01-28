#include "MyCommon.h"
#include "Settings.h"
#include "Packet.h"
#include "MySocket.h"



void setup() {
  Serial.begin(9600);
  EEPROM.begin(128);
  GetSettings();
  WifiInit();
  WebSocketInit();

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
  #endif
}

#ifdef CONTROLLER_LED
  void TaskLed( void * pvParameters ){
    for(;;) {
      Ws2812Loop ();
      delay (0);
    } 
  }
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
  #endif
}