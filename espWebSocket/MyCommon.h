#define HOME
//#define WORK

#include <stdint.h>
#include "Base64.h"
#ifdef ESP32
  SET_LOOP_TASK_STACK_SIZE(45*1024); // Задаем размер стека для Loop
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <WebSocketsClient.h>
WebSocketsClient webSocket;

#define CONTROLLER_TELEMETRY
//#define TEMPERATURE_SENSOR
//#define CO2_SENSOR 

//#define CONTROLLER_LED

#ifdef CONTROLLER_TELEMETRY
  #ifdef TEMPERATURE_SENSOR
    #define DHT22Sensor
    ////// ТУТ ПО ИДЕЕ МОЖНО ДОБАВЛЯТЬ ЕЩЕ КАКИЕ НИБУДЬ ДАТЧИКИ//////////////////
  #endif

  #ifdef CO2_SENSOR  
    #define MQ135Sensor
    ////// ТУТ ПО ИДЕЕ МОЖНО ДОБАВЛЯТЬ ЕЩЕ КАКИЕ НИБУДЬ ДАТЧИКИ//////////////////
  #endif
#endif

#ifdef DHT22Sensor
  #include <DHT22.h>
  #define pinDATA 4 
  DHT22 dht22(pinDATA);
#endif  

#ifdef MQ135Sensor
  #include <MQ135.h>
  // Делал калибровку на свежем воздухе
  uint64_t TimerCallibrate = millis() + 15000;
  MQ135 gasSensor = MQ135(A0, 26.33);
  void CallibrateMQ135 ()
  {
    float R = 1.00;
    while (gasSensor.getPPM() < 400) {
      R = R + 1.11;
      gasSensor.SetRZero(R);
    }
  }
#endif

//////////////////////////////// WIFI НАСТРОЙКИ ///////////////////////////////////////
#ifdef WORK
  const char* ssid = "Wentor";
  const char* password = "0hqWs40IsY";
#endif

#ifdef HOME
  const char* ssid = "TP-Link_467D";
  const char* password = "66484608";
#endif
//////////////////////////////////////////////////////////////////////////////////////

///////////////////////// ПОДКЛЮЧЕНИЕ К СЕРВЕРУ /////////////////////////////////////
#ifdef HOME 
  const char* ADDR = "192.168.0.105";
#endif

#ifdef WORK
  const char* ADDR = "192.168.8.83";
#endif

String URL = String("/ws?key=") + base64::encode("ESP32");
const uint16_t PORT = 8888;
//////////////////////////////////////////////////////////////////////////////////////

////////////////////////////ПРОЧИЕ НАСТРОЙКИ/////////////////////////////////////////
#ifdef TEMPERATURE_SENSOR
  uint64_t Timer1   = millis() + 10000;
  uint32_t TimerTempAndHum   = 0;
#endif

#ifdef CO2_SENSOR
  uint64_t Timer2   = millis() + 10000;
  uint32_t TimerCO2 = 0;
#endif

 
