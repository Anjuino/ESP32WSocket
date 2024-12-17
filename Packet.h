enum {
  START,
  DATA,
} TypePacket;

enum {
  TEST,
  TEMPHUM,
} TypeDevice;

struct PacketStart {
  uint8_t Packet;
  uint64_t ChipID;
  uint8_t  DeviceType;
};
