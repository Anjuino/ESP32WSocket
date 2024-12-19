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

void ParsePacket (uint8_t * payload, uint64_t length);

