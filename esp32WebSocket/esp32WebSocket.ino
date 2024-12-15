#include "MyCommon.h"

#include "MySocket.h"

void setup() {

  Serial.begin(115200);
  WifiInit ();
  WebSocketInit ();
}

void loop() {

  webSocket.loop();
  if (millis() > Timer) {
    if (webSocket.isConnected()) {
      Timer = millis() + 25000;
      webSocket.sendTXT("Hello from ESP32!");
    }
  }
}