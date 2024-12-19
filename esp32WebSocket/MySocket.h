#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Packet.h>

WebSocketsClient webSocket;

TaskHandle_t WebSocketTask;
void WebSocket( void * parameter) {
  uint64_t Timer = 0;
  while (true) {
    webSocket.loop();
    if (webSocket.isConnected()) {
      if (OneFlag) R = 0; G = 140; B = 0; OneFlag = false;
    } else {
      if (!OneFlag) R = 140; G = 0; B = 0; OneFlag = true;
    }
    delay(0);
  }
}

void SendPacketStart () 
{
  PacketStart Packet;

  Packet.Packet     = START;
  Packet.UID        = 2;
  Packet.ChipID     = ESP.getEfuseMac();
  Packet.DeviceType = TELEMETRY;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры

  uint8_t Data[DataSize];              // Выделяю память
  memcpy(Data, &Packet, DataSize);     // Копирую пакет на отправку

  webSocket.sendBIN(Data, DataSize);   // Отправялю пакет на сервер
}

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
  xTaskCreatePinnedToCore(WebSocket, "WebSocketTask", 1024*20, NULL, 0, &WebSocketTask, 0);   // Запускаю задачу для webSocket
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
  Freg = 300;
}