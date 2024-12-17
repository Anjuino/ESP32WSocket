#include <stdint.h>

//const char* ssid = "Wentor";
//const char* password = "0hqWs40IsY";

const char* ssid = "TP-Link_467D";
const char* password = "66484608";

const char* ADDR = "192.168.0.105";
//const char* ADDR = "192.168.8.83";

const char* URL = "/ws";
const uint16_t PORT = 8888;

uint64_t Timer = 0;

uint64_t TimerLed = 0;
bool LedFlag      = true;
uint8_t NumPinLed = 21;

uint8_t R = 0;
uint8_t G = 0;
uint8_t B = 140;

void LedStatus (uint8_t R, uint8_t G, uint8_t B)
{
  if (TimerLed < millis ()) {
    TimerLed = millis () + 500;

    if (LedFlag) {
      LedFlag = false;
      neopixelWrite (NumPinLed, R, G, B);
    }
    else {
      LedFlag = true;
      neopixelWrite (NumPinLed, 0, 0, 0);
    }
  } 
}