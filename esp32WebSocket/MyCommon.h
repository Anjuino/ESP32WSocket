#include <stdint.h>
#include "Base64.h"
#include "Settings.h"

#include <WiFi.h>
#include <WebSocketsClient.h>
WebSocketsClient webSocket;

//#define HOME
#define WORK

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



 
