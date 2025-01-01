#include "MyCommon.h"
#include "Settings.h"
#include "Packet.h"
#include "MySocket.h"

void setup() {
  Serial.begin(9600);
  EEPROM.begin(128);
  GetSettings();
  WifiInit();
  WebSocketInit();
}


void loop() {

  webSocket.loop();

  #ifdef DHT22Sensor
    if (millis() > Timer1) {
      Timer1 = millis() + TimerDHT;
      SendPacketTepmHum();
    }
  #endif  

  #ifdef MQ135Sensor
    if (millis() > Timer2) {
      Timer2 = millis() + TimerMQ135;
      SendPacketCO2();
    }
  #endif
}