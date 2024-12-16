#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
      case WStype_CONNECTED:
        Serial.println("Connected to server");
        break;

      case WStype_DISCONNECTED:
        Serial.println("Disconnected from server");
        break;

      case WStype_TEXT:
        Serial.printf("Received message: %s\n", payload);
        break;
  }
}

void WebSocketInit () {
  webSocket.begin(ADDR, PORT, URL);
  webSocket.onEvent(webSocketEvent);
  //webSocket.setReconnectInterval(ReconnectInterval);
}

void WifiInit () {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
  }
}