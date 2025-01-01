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
//#define CONTROLLER_LED


#define HOME
//#define WORK

#ifdef CONTROLLER_TELEMETRY
  #define DHT22Sensor
  //#define MQ135Sensor
#endif

#ifdef DHT22Sensor
  #include <DHT22.h>
  #define pinDATA 4 
  DHT22 dht22(pinDATA);
#endif  

#ifdef MQ135Sensor
  #include <MQ135.h>
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
uint64_t Timer1   = millis() + 10000;
uint64_t Timer2   = millis() + 10000;
uint32_t TimerDHT   = 0;
uint32_t TimerMQ135 = 0;

 
