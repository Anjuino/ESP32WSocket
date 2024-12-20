/*#include "HardwareSerial.h"
#include "Packet.h"
#include "Settings.h"

void ParsePacket (uint8_t * payload, uint64_t length)
{
  Serial.print("Размер пакета: ");
  Serial.println(length);

  uint16_t SizeData = length - 1;              // Определяю размер пакета без типа
  uint8_t PacketData[SizeData];                // Определяю массив под данные без типа пакета
  memcpy(PacketData, payload + 1, SizeData);   // Копирую в новый массив данные пакета без типа пакета

              // Тип пакета
  switch (TypePacketResponse)
  {
    case SET_UID:
    {
      Serial.println("Пакет с UID");
      PacketUID ReceivedPacket;
      memcpy(&ReceivedPacket, PacketData, sizeof(PacketUID));
      Serial.printf("UID: %d\n", ReceivedPacket.UID);
      Settings.UID = ReceivedPacket.UID;
      WriteSettings ();
    }
  }
}*/