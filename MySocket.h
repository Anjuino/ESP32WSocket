#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Packet.h>

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
      case WStype_CONNECTED:
      {
        Serial.println("Connected to server");

        PacketStart Packet;

        Packet.Packet     = START;
        Packet.ChipID     = ESP.getEfuseMac();
        Packet.DeviceType = TEST;

        Serial.println(TEST);
        Serial.println(ESP.getEfuseMac());

        uint16_t DataSize = sizeof(Packet);
        uint8_t Data[DataSize];
        memcpy(Data, &Packet, DataSize);
        webSocket.sendBIN(Data, DataSize);

        //Тут надо отправить пакет с UID чтобы сервер записал у себя его
        /*String TypePacket = "START ";
        webSocket.sendTXT("TypePacket: " + TypePacket + "ChipID: " + String(ESP.getEfuseMac()) + " SensorName: " + NameDevice);*/
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
  }
}

void WebSocketInit () {
  webSocket.begin(ADDR, PORT, URL);
  webSocket.onEvent(webSocketEvent);
}

void WifiInit () {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
  }
}