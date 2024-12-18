///////////////////////////////////////////// ОПИСАНИЕ ПАКЕТОВ ОТПРАВЛЯЕМЫХ НА СЕРВЕР //////////////////////////////////////////////

////////////// ТИПЫ ПАКЕТОВ////////////////////////
enum {
  START,
  DATA,
} TypePacket;

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
  uint64_t ChipID;    
  uint8_t  DeviceType;
};
#pragma pack(pop) // Возвращаем предыдущее выравнивание

///////////////////////////////ПАКЕТ С ДАННЫМИ ////////////////////////////////////////////////////////
#pragma pack(push, 1) // Устанавливаем выравнивание в 1 байт
struct PacketTelemetry {
  uint8_t  Packet;     
  uint16_t DID;    
  uint8_t  DeviceType;
  float Temperature;
  float Humidity;  
};
#pragma pack(pop) // Возвращаем предыдущее выравнивание
