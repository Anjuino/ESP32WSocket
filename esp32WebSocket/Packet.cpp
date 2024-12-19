#include "HardwareSerial.h"
#include "Packet.h"

void ParsePacket (uint8_t * payload, uint64_t length)
{
  Serial.print("Размер пакета: ");
  Serial.println(length);

  uint16_t SizeData = length - 1;              // Определяю размер пакета без типа
  uint8_t PacketData[SizeData];                // Определяю массив под данные без типа пакета
  memcpy(PacketData, payload + 1, SizeData);   // Копирую в новый массив данные пакета без типа пакета

  uint8_t TypePacket = payload[0];             // Тип пакета
  if (TypePacket == 3) {
    Serial.println("Пакет с UID");
    PacketUID ReceivedPacket;
    memcpy(&ReceivedPacket, PacketData, sizeof(PacketUID));
    Serial.printf("UID: %d\n", ReceivedPacket.UID);
    // Тут надо писать UID в EEPROM
  }
}