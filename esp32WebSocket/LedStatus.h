uint64_t TimerLed = 0;
bool LedFlag      = true;
uint8_t NumPinLed = 21;
uint16_t Freg = 500;

uint8_t R = 0;
uint8_t G = 0;
uint8_t B = 140;
bool OneFlag = false;   // Нужен для разового определения цвета светодиода

void LedStatus (uint8_t R, uint8_t G, uint8_t B)
{
  if (TimerLed < millis ()) {
    TimerLed = millis () + Freg;

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

TaskHandle_t LedStatusTask;
void LedStatusloop(void * parameter) {
  while (true) {
    LedStatus (R, G, B);
    delay(0);
  }
}

void LedInit ()
{
  xTaskCreatePinnedToCore(LedStatusloop, "LedStatusTask", 4096, NULL, 0, &LedStatusTask, 0);  // Запускаю задачу для светодиода
}
