#define HOME
//#define WORK

#include <stdint.h>
#include "Base64.h"
#include <EEPROM.h>

#ifdef ESP32
  SET_LOOP_TASK_STACK_SIZE(20*1024); // Задаем размер стека для Loop
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <WebSocketsClient.h>
WebSocketsClient webSocket;

/////////////////////////////////////////////////////////////КОНТРОЛЛЕР ТЕЛЕМЕТРИИ/////////////////////////////////////////////////////////////
//#define CONTROLLER_TELEMETRY
//#define TEMPERATURE_SENSOR
//#define CO2_SENSOR
//#define PRESSURE_SENSOR
bool IsTempAndHumSensor = true;
bool IsCO2Sensor        = false;
bool IsPressureSensor   = true;
bool IsInaSensor        = false;

#ifdef CONTROLLER_TELEMETRY
  #ifdef TEMPERATURE_SENSOR
    //#define DHT22Sensor
    #define BME280Sensor
    ////// ТУТ ПО ИДЕЕ МОЖНО ДОБАВЛЯТЬ ЕЩЕ КАКИЕ НИБУДЬ ДАТЧИКИ//////////////////
  #endif

  #ifdef CO2_SENSOR  
    #define MQ135Sensor
    ////// ТУТ ПО ИДЕЕ МОЖНО ДОБАВЛЯТЬ ЕЩЕ КАКИЕ НИБУДЬ ДАТЧИКИ//////////////////
  #endif

  #ifdef PRESSURE_SENSOR
    #define BME280Sensor  
  #endif 
#endif

#ifdef DHT22Sensor
  #include <DHT22.h>
  #define pinDATA 4 
  DHT22 dht22(pinDATA);
#endif  

#ifdef BME280Sensor
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>
  Adafruit_BME280 bme;
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

/////////////////////////////////////////////////////////////КОНТРОЛЛЕР СВЕТА/////////////////////////////////////////////////////////////
//#define CONTROLLER_LED
//#define DETECTED_SENSOR
//#define LIGHT_SENSOR
bool IsDetectedSensor = false;
bool IsLightSensor    = false;
#ifdef CONTROLLER_LED
  #include "Ws2812.h"
  TaskHandle_t Task1;
  #ifdef DETECTED_SENSOR
    // ТУТ ДЛЯ ДАТЧИКА ДВИЖЕНИЯ КОД, СКОРЕЕ ВСЕГО ФУНКЦИЯ
    TaskHandle_t TaskDetected;
    bool Automode = true;    // это с сервера и в настройках должно ли вообще работать освещение в авто режиме
    bool IsDetectedMove = false;
    pinMode(33, INPUT_PULLUP);

    void IRAM_ATTR MoveDetected() {
      IsDetectedMove = true;
    }

    attachInterrupt(33, MoveDetected, RISING);
  #endif
  #ifdef LIGHT_SENSOR
    analogReadResolution(12);
    // ТУТ ДЛЯ ДАТЧИКА ОСВЕЩЕИЯ КОД, СКОРЕЕ ВСЕГО ФУНКЦИЯ
    uint16_t ReadLight () {
      return analogRead(36);
    }
  #endif
#endif

/////////////////////////////////////////////////////////////КОНТРОЛЛЕР ПАНЕЛЬ УПРАВЛЕНИЯ/////////////////////////////////////////////////////////////
//#define PANELCONTROL
#ifdef PANELCONTROL

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
  uint64_t Timer1   = millis() + 20000;
  uint32_t TimerTempAndHum   = 0;
#endif

#ifdef CO2_SENSOR
  uint64_t Timer2   = millis() + 20000;
  uint32_t TimerCO2 = 0;
#endif

#ifdef PRESSURE_SENSOR
  uint64_t Timer3 = millis() + 20000;
  uint32_t TimerPressure = 0;
#endif
