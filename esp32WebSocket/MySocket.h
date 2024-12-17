#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Packet.h>

WebSocketsClient webSocket;

void SendTestPack () {
  PacketStart Packet;

  Packet.Packet     = START;
  Packet.ChipID     = ESP.getEfuseMac();
  Packet.DeviceType = TEST;

  Serial.println(ESP.getEfuseMac());

  uint16_t DataSize = sizeof(Packet);

  Serial.println(DataSize);

  uint8_t Data[DataSize];
  memcpy(Data, &Packet, DataSize);

  webSocket.sendBIN(Data, DataSize);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
      case WStype_CONNECTED:
      {
        Serial.println("Connected to server");
        SendTestPack ();
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