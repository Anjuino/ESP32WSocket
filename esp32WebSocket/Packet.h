#include <stdint.h>
///////////////////////////////////////////// ОПИСАНИЕ ПАКЕТОВ ОТПРАВЛЯЕМЫХ НА СЕРВЕР //////////////////////////////////////////////

////////////// ТИПЫ ПАКЕТОВ////////////////////////
// Пакеты на сервер
enum {
  START,
  DATA,
} TypePacketSend;

// Пакеты с сервера
enum {
  SET_UID,
  COMMAND
} TypePacketResponse;

///////////// ТИПЫ УСТРОЙСТВ//////////////////////
enum {
  TEST,
  TELEMETRY,
  LEDCONTROL,
} TypeDevice;

///////////////////// СТАРТОВЫЙ ПАКЕТ///////////////////////
#pragma pack(push, 1) // Устанавливаем выравнивание в 1 байт
struct PacketStart {
  uint8_t  Packet;
  uint8_t  UID;     
  uint64_t ChipID;    
  uint8_t  DeviceType;
};

///////////////////////////Пакет от сервера с UID////////////////////////
struct PacketUID {   
  uint8_t  UID;    
};

///////////////////////////////ПАКЕТ С ДАННЫМИ ////////////////////////////////////////////////////////
struct PacketTelemetry {
  uint8_t  Packet;     
  uint16_t UID;    
  uint8_t  DeviceType;
  float Temperature;
  float Humidity;  
};
#pragma pack(pop) // Возвращаем предыдущее выравнивание

//void ParsePacket (uint8_t * payload, uint64_t length);
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
      Serial.printf("UIDFlash: %d\n", Settings.UID);
    }
  }
}

