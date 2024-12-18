#include "MyCommon.h"
#include "LedStatus.h"
#include "MySocket.h"

void setup() {
  Serial.begin(115200);
  LedInit();
  WifiInit();
  WebSocketInit();
}

void loop() {

}