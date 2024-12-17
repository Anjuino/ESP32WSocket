enum {
  START,
  DATA,
} TypePacket;

enum {
  TEST,
  TEMPHUM,
} TypeDevice;

#pragma pack(push, 1) // Устанавливаем выравнивание в 1 байт
struct PacketStart {
  uint8_t  Packet;     
  uint64_t ChipID;    
  uint8_t  DeviceType; 
};
#pragma pack(pop) // Возвращаем предыдущее выравнивание
