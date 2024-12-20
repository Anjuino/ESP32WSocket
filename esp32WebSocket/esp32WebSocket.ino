#include "MyCommon.h"
#include "LedStatus.h"
#include <Packet.h>
#include "MySocket.h"

void setup() {
  Serial.begin(9600);
  EEPROM.begin(128);
  LedInit();
  GetSettings();
  //Serial.printf("UIDFlashLoad: %d\n", Settings.UID);
  WifiInit();
  WebSocketInit();
}

void loop() {

}