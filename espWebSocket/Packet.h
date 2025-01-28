#include <stdint.h>
///////////////////////////////////////////// ОПИСАНИЕ ПАКЕТОВ ОТПРАВЛЯЕМЫХ НА СЕРВЕР //////////////////////////////////////////////

////////////// ТИПЫ ПАКЕТОВ////////////////////////
// Пакеты на сервер
enum {
  START,
  ///////////////// Контроллер телеметрии
  DATA_Temp_AND_Hum,
  DATA_CO2ppm,
  DATA_TIMER_SENSOR,
  ////////////////// Контроллер для светодоидной ленты
  DATA_LED_STATE = 10,
} TypePacketSend;

// Пакеты с сервера
enum {
  SET_UID,
  GET_Temp_AND_Hum,
  GET_CO2ppm,
  COMMAND,
} TypePacketResponse;



///////////// ТИПЫ УСТРОЙСТВ//////////////////////
enum {
  TEST,
  TELEMETRY,
  LEDCONTROL,
} TypeDevice;

///////////////////// СТАРТОВЫЙ ПАКЕТ///////////////////////

//////////////////// ТИПЫ КОМАНД////////////////////////////
enum {
  Test,
  ////////////////////// Для телеметрии ////////////////////
  SET_TIME_TEMP_AND_HUM,
  SET_TIME_CO2,
  LEDBLINK,
  GET_TIMER_SENSOR,
  ///////////////////// Для управления лентой ////////////////////
  SET_BLIND,
  SET_SPEED,
  SET_MODE,
  LED_GET_STATE,
  SET_LED_COUNT,
} TypeCommand;


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
struct PacketTemp_Hum {
  uint8_t  Packet;     
  uint8_t  UID;
  bool IsNeedWriteDataBase;
  bool IsSensor;    
  float Temperature;
  float Humidity;  
};

struct PacketCO2 {
  uint8_t  Packet; 
  uint8_t  UID;
  bool IsNeedWriteDataBase;
  bool IsSensor;       
  uint16_t CO2ppm;
};

struct PacketCommand {
  uint8_t  Command;
  uint32_t CommandData;
};

struct PacketTimerSensor {
  uint8_t  Packet; 
  uint8_t UID;
  uint32_t TimerSensor;
};

struct PacketLedState {
  uint8_t  Packet; 
  uint8_t  UID;
  uint8_t  Mode;
  uint32_t Blind;
  uint32_t Speed;
  uint8_t  R;
  uint8_t  G;
  uint8_t  B;
};

#pragma pack(pop) // Возвращаем предыдущее выравнивание

void SendPacket(uint8_t *hData, uint16_t Size)
{
  uint8_t Data[Size];                  // Выделяю память
  memcpy(Data, hData, Size);           // Копирую пакет на отправку

  webSocket.sendBIN(Data, Size);       // Отправялю пакет на сервер
}

void SendPacketTepmHum(bool IsNeedWriteDataBase = true)
{ 
  float Humidity    = -1;    
  float Temperature = -273;
  bool IsSensor;
  #ifdef DHT22Sensor
    IsSensor = true;
    uint8_t GetTry = 0;

    while (GetTry < 3) {
      Humidity = dht22.getHumidity();
      //Serial.println(Humidity);
      if (Humidity > 0) {
        GetTry = 0;
        break;
      }
      GetTry++;
    }

    GetTry = 0;
    while (GetTry < 3) {
      Temperature = dht22.getTemperature();
      //Serial.println(Temperature);
      if (Temperature > -273) {
        GetTry = 0;
        break;
      } 
      GetTry++;
    }

  #else
    IsSensor = false;
  #endif  

  PacketTemp_Hum Packet;

  Packet.Packet              = DATA_Temp_AND_Hum;
  Packet.UID                 = Settings.UID;
  Packet.IsNeedWriteDataBase = IsNeedWriteDataBase;
  Packet.IsSensor            = IsSensor;
  Packet.Temperature         = Temperature;
  Packet.Humidity            = Humidity;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketCO2(bool IsNeedWriteDataBase = true) 
{
  uint16_t CO2ppm = 0;
  bool IsSensor;
  
  #ifdef MQ135Sensor
    IsSensor = true;
    //CO2ppm = random(400, 700);
    Serial.println(gasSensor.getRZero());
    CO2ppm = gasSensor.getPPM();
  #else
    IsSensor = false;
  #endif  

  PacketCO2 Packet;

  Packet.Packet              = DATA_CO2ppm;
  Packet.UID                 = Settings.UID;
  Packet.IsNeedWriteDataBase = IsNeedWriteDataBase;
  Packet.IsSensor            = IsSensor;
  Packet.CO2ppm              = CO2ppm;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketTimerSensor(uint32_t TypeSensor)
{
  uint32_t Timer = 1;  // Значение по умолчанию, сигнал о том что таймера нет, значит и нет датчика
  #ifdef TEMPERATURE_SENSOR
    if(TypeSensor == 1) Timer = Settings.TimerTempAndHum;
  #endif
  #ifdef CO2_SENSOR
    if(TypeSensor == 2) Timer = Settings.TimerCO2;
  #endif

  PacketTimerSensor Packet;

  Packet.Packet              = DATA_TIMER_SENSOR;
  Packet.UID                 = Settings.UID;
  Packet.TimerSensor         = Timer;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketLedState()
{
  PacketLedState Packet;

  Packet.Packet     = DATA_LED_STATE;
  Packet.UID        = Settings.UID;
  Packet.Mode       = step;
  Packet.Blind      = BlindLed;
  Packet.Speed      = Speed;
  Packet.R          = r1;
  Packet.G          = g1;  
  Packet.B          = b1;    

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketStart() 
{
  PacketStart Packet;

  Packet.Packet     = START;
  Packet.UID        = Settings.UID;
  #ifdef ESP32
    Packet.ChipID     = ESP.getEfuseMac();
  #else
    Packet.ChipID     = ESP.getChipId();
  #endif  
  Packet.DeviceType = LEDCONTROL;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPong()
{
  webSocket.sendBIN((const uint8_t*)NULL, 0);
  delay(0);
}

void ParsePacket(uint8_t * payload, uint64_t length)
{
  Serial.print("Размер пакета: ");
  Serial.println(length);
  uint8_t TypePacketResponse = payload[0];     // Тип пакета
  //Serial.println("Принял пакет:");
  //Serial.print(TypePacketResponse);

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
      break;
    }

    #ifdef CONTROLLER_TELEMETRY
      case GET_Temp_AND_Hum:
      {
        Serial.println("Запрос Temp и Hum");
        SendPacketTepmHum(false);
        break;
      }

      case GET_CO2ppm:
      {
        Serial.println("Запрос CO2");
        SendPacketCO2(false);
        break;
      }
    #endif

    case COMMAND:
    {
      Serial.println("Пришла команда");
      PacketCommand ReceivedPacket;
      memcpy(&ReceivedPacket, PacketData, sizeof(PacketCommand));

      uint8_t TypeCommand = ReceivedPacket.Command;
      Serial.println(ReceivedPacket.CommandData);

      switch (TypeCommand)
      {
        #ifdef CONTROLLER_TELEMETRY
          case GET_TIMER_SENSOR:
          {
            if (ReceivedPacket.CommandData) SendPacketTimerSensor(ReceivedPacket.CommandData);
            break;
          }

          #ifdef TEMPERATURE_SENSOR
            case SET_TIME_TEMP_AND_HUM:
            {
              if (ReceivedPacket.CommandData) {
                TimerTempAndHum = ReceivedPacket.CommandData;
                Settings.TimerTempAndHum = TimerTempAndHum;
                Serial.println(Settings.TimerTempAndHum);
                WriteSettings();
              }

              break;
            }
          #endif

          #ifdef CO2_SENSOR
            case SET_TIME_CO2:
            {
              if (ReceivedPacket.CommandData) {
                TimerCO2 = ReceivedPacket.CommandData;
                Settings.TimerCO2 = TimerCO2;
                WriteSettings();
              }
              break;
            }
          #endif

        #endif

        #ifdef CONTROLLER_LED
          case SET_BLIND:
          {
            Ws2812SetBlind (ReceivedPacket.CommandData);
            break; 
          }

          case SET_SPEED:
          {
            Ws2812SetSpeed (ReceivedPacket.CommandData);
            break;
          }

          case SET_MODE:
          { 
            SetMode(ReceivedPacket.CommandData);
            break;
          }

          case LED_GET_STATE:
          {
            SendPacketLedState();
            break;
          }

          case SET_LED_COUNT:
          {
            uint32_t CountLed = ReceivedPacket.CommandData;
            Serial.println(CountLed);
            Ws2812SetColor (0, 0, 0);
            if (CountLed > 700) CountLed = 700;
            if (CountLed < 0)   CountLed = 0;
            Serial.println(CountLed);
            strip.updateLength (CountLed);

            Settings.CountLed = CountLed;
            WriteSettings();
            FlagOneOn = true;
            break;
          }

        #endif
      }
      break;
    }
  }
  return;
}
