#include "MyCommon.h"
#include "LedStatus.h"
#include "MySocket.h"

void setup() {
  Serial.begin(115200);
  LedInit();
  // Тут добавить выгрузку данных из EEPROM (UID)
  WifiInit();
  WebSocketInit();
}

void loop() {

}