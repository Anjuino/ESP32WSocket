#include "MyCommon.h"
#include "MySocket.h"

TaskHandle_t LedStatusTask;
void LedStatus( void * parameter) {
  while (true) {
    LedStatus (R, G, B);
  }
}

TaskHandle_t WebSocketTask;
void WebSocket( void * parameter) {
  while (true) {
    webSocket.loop();
    if (webSocket.isConnected()) {
      R = 0; G = 140; B = 0;
      if (millis() > Timer) {
        Timer = millis() + 15000;
        SendTestPack ();
      }
    } else {
      R = 140; G = 0; B = 0;
    }
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(LedStatus, "LedStatusTask", 4096, NULL, 0, &LedStatusTask, 0); 
  WifiInit();
  WebSocketInit();
  xTaskCreatePinnedToCore(WebSocket, "WebSocketTask", 1024*20, NULL, 0, &WebSocketTask, 0);
}

void loop() {

}