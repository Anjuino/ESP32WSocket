#include "MyCommon.h"
#include "MySocket.h"

TaskHandle_t Task1;
void Task1code( void * parameter) {
 
  for(;;) {
  LedStatus (R, G, B);
  }
 
}
void setup() {

  Serial.begin(115200);
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 0, &Task1, 0); 
  WifiInit ();
  WebSocketInit ();
}

void loop() {

  webSocket.loop();

  if (webSocket.isConnected()) {
    R = 0;
    G = 140;
    B = 0;

    if (millis() > Timer) {
      Timer = millis() + 25000;
      webSocket.sendTXT("Hello from ESP32!");
    }

  } else {
    R = 140;
    G = 0;
    B = 0;
  }
}