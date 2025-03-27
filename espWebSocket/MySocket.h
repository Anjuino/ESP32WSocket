uint64_t TimerToReset = 0;
bool TimerIsActivated = false;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {    // Обработка событий от сервера
  switch(type) {
      case WStype_CONNECTED:
      {
        Serial.println("Connected to server");
        TimerToReset = 0;
        TimerIsActivated = false;
        SendPacketStart ();
        delay(500);
        SendPacketDeviceConfig();
        // Тут отправить пакет с конифгом (будет разный для разных устройств)
        break;
      }

      case WStype_DISCONNECTED:
      { 
        Serial.println("Disconnected from server");
        if (!TimerIsActivated) {
          TimerToReset = millis() + 600000;
          TimerIsActivated = true;
        }
        if (millis() > TimerToReset) ESP.restart();
        delay(10);
        break;
      }

      case WStype_PING:
      { 
        SendPong();
        break;
      }

      case WStype_PONG:
      {
        break;
      }

      case WStype_TEXT:
      {
        Serial.printf("Received message: %s\n", payload);
        //ParseString (payload, length);   // Разбор принятых данных от сервера в виде строки
        break;
      }

      case WStype_BIN: 
      { 
        Serial.println("Received binary data");
        ParsePacket (payload, length);   // Разбор принятых данных от сервера в виде бинарных данных
        break;
      }
  }
}

void WebSocketInit () 
{
  webSocket.begin(ADDR, PORT, URL);      // Инициирую подключение к серверу
  webSocket.onEvent(webSocketEvent);     // Указываю обработчик для событий
}

void WifiInit ()  // Подключаюсь к сети
{
  WiFi.begin(ssid, password);
  uint64_t Timer = millis();
  while (WiFi.status() != WL_CONNECTED) {
      if (millis () > Timer) {
        Timer = millis() + 200;
        Serial.println("Connecting to WiFi...");
      }
  }
  Serial.println("WIFI Connect");
}