void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {    // Обработка событий от сервера
  switch(type) {
      case WStype_CONNECTED:
      {
        Serial.println("Connected to server");
        SendPacketStart ();
        break;
      }

      case WStype_DISCONNECTED:
      { 
        Serial.println("Disconnected from server");
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
        break;
      }

      case WStype_BIN: 
      { 
        Serial.println("Received binary data");
        ParsePacket (payload, length);   // Разбор принятых данных от сервера
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