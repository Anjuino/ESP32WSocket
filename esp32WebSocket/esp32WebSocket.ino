#include "MyCommon.h"
#include "LedStatus.h"
#include <Packet.h>
#include "MySocket.h"

uint64_t Timer = 0;
uint64_t Timer1 = 0;

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
  if (millis() > Timer) {
    Timer = millis() + 600000;
    SendPacketTepmHum();
  }

  if (millis() > Timer1) {
    Timer1 = millis() + 660000;
    SendPacketCO2();
  }
}