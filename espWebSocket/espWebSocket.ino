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

  #ifdef ENS160Sensor
    ENS160.begin();
    ENS160.setPWRMode(ENS160_STANDARD_MODE);
    ENS160.setTempAndHum(25.0, 30.0);
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
      pinMode(1, INPUT_PULLUP);
      attachInterrupt(1, MoveDetected, RISING);   
    #endif
    #ifdef LIGHT_SENSOR
      analogReadResolution(12);
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
            Serial.println("Есть движение");
            #ifdef LIGHT_SENSOR
              if (ReadLight() < LightLimit || IsWork) {            // тут уровень освещения и если система уже сработала
            #else
              if (IsWork) {
            #endif
                if (!IsWork) {
                  SetMode(4194276895);
                  Serial.println("Включил");
                }
                IsWork = true;
                TimerWork = millis() + 120000;            // Обновил время

                Serial.println("Обновил время");
            }           
          }

          if (IsWork) {
            if (millis () > TimerWork) {
              IsWork = false;
              TimerWork = 0;
              Ws2812SetMode(250);
              Serial.println("Выключил ленту, сбросил флаги");
            }
          }
        }
      }
    }
  #endif
#endif

#ifdef TEMPERATURE_SENSOR
  void AlertTemp (void) {
    if (TempAndHumAlert) {
      if (millis() > TimerTempAndHumAlert) {
        TimerTempAndHumAlert = millis() + 60000;
        #ifdef DHT22Sensor
          if (dht22.getTemperature() > MaxLimitT || dht22.getTemperature() < MinLimitT) {
        #endif
        #ifdef BME280Sensor
          if (bme.readTemperature() > MaxLimitT || bme.readTemperature() < MinLimitT) {
        #endif  
          Serial.println("Тревога по Температуре");
          TimerTempAndHumAlert = millis() + 600000;    // Перевожу время на 10 минут вперед, чтобы не спамить сервер
          SendPacketTepmHum(false, true);
        }
      }
    }
  }
#endif

#ifdef CO2_SENSOR
  void AlertCO2 (void) {
    if (CO2Alert) {                             // Если включена тревога и при превышении порогового значения СO2, то будет отправляться пакет с признаком тревоги
      if (millis() > TimerCO2Alert) {
        TimerCO2Alert = millis() + 60000;
        #ifdef MQ135Sensor
          if (gasSensor.getPPM() > MaxLimitCO2) {
        #endif
        #ifdef ENS160Sensor
          if (ENS160.getECO2() > MaxLimitCO2) {
        #endif    
          Serial.println("Тревога по Co2");
          TimerCO2Alert = millis() + 600000;    // Перевожу время на 10 минут вперед, чтобы не спамить сервер
          SendPacketCO2(false, true);
        }
      }
    }
  }
#endif

void loop() {

  webSocket.loop();

  #ifdef CONTROLLER_TELEMETRY
    #ifdef TEMPERATURE_SENSOR
      AlertTemp();
      if (millis() > Timer1) {
        Timer1 = millis() + TimerTempAndHum;
        SendPacketTepmHum();
      }
    #endif  

    #ifdef CO2_SENSOR
      AlertCO2();
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