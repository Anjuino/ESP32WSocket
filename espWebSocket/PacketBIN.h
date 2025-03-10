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
  DATA_PRESSURE,
  ////////////////// Контроллер для светодоидной ленты
  DATA_LED_STATE = 10,
  DEVICE_CONFIG  = 20,
  STATE_ALERT = 30,
  DATA_LIMIT_VALUE,
} TypePacketSend;

// Пакеты с сервера
enum {
  SET_UID,
  GET_Temp_AND_Hum,
  GET_CO2ppm,
  COMMAND,
  GET_PRESSURE,
} TypePacketResponse;



///////////// ТИПЫ УСТРОЙСТВ//////////////////////
enum {
  CONTROLPANEL,
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
  /////////////////////// Для телеметрии //////////////////////////
  SET_TIME_PRESSURE,
  //////////////////////  Для тревог //////////////////////////////
  SET_MODE_WORK,
  SET_ALERT,
  GET_ALERT,
  SET_LIMIT,
  GET_LIMIT,
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
  bool IsAlarm;      
  float Temperature;
  float Humidity;  
};

struct PacketCO2 {
  uint8_t  Packet; 
  uint8_t  UID;
  bool IsNeedWriteDataBase;
  bool IsSensor;
  bool IsAlarm;         
  uint16_t CO2ppm;
};

struct PacketPressure {
  uint8_t  Packet; 
  uint8_t  UID;
  bool IsNeedWriteDataBase;
  bool IsSensor;
  bool IsAlarm;         
  float Pressure;
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

struct PacketDataAlert {
  uint8_t  Packet; 
  uint8_t UID;
  uint8_t TypeSensor;
  bool AlertIsOn;
};

struct PacketDataLimit {
  uint8_t  Packet; 
  uint8_t UID;
  uint8_t TypeSensor;
  uint8_t LimitType;
  int16_t LimitValue;
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
  #ifdef DETECTED_SENSOR
    uint8_t ModeWork;
  #endif
};

struct PacketDeviceConfig {
  uint8_t  Packet;     
  uint8_t  DeviceType;
  uint8_t  UID; 
  #ifdef CONTROLLER_TELEMETRY
    bool IsTempAndHumSensor;
    bool IsCO2Sensor;
    bool IsPressureSensor;
    bool IsInaSensor;
  #endif
  #ifdef CONTROLLER_LED
    bool IsDetectedSensor;
    bool IsLightSensor;
  #endif
};

#pragma pack(pop) // Возвращаем предыдущее выравнивание

void SendPacket(uint8_t *hData, uint16_t Size)
{
  uint8_t Data[Size];                  // Выделяю память
  memcpy(Data, hData, Size);           // Копирую пакет на отправку

  webSocket.sendBIN(Data, Size);       // Отправялю пакет на сервер
}

void SendPacketTepmHum(bool IsNeedWriteDataBase = true, bool IsAlarm = false)
{ 
  float Humidity    = -1;    
  float Temperature = -273;
  bool IsSensor = false;
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
  #endif

  #ifdef BME280Sensor
    IsSensor = true;
    Humidity    = bme.readHumidity();
    Temperature = bme.readTemperature();
  #endif    

  PacketTemp_Hum Packet;

  Packet.Packet              = DATA_Temp_AND_Hum;
  Packet.UID                 = Settings.UID;
  Packet.IsNeedWriteDataBase = IsNeedWriteDataBase;
  Packet.IsSensor            = IsSensor;
  Packet.IsAlarm             = IsAlarm; 
  Packet.Temperature         = Temperature;
  Packet.Humidity            = Humidity;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketCO2(bool IsNeedWriteDataBase = true, bool IsAlarm = false) 
{
  uint16_t CO2ppm = 0;
  bool IsSensor = false;
  
  #ifdef MQ135Sensor
    IsSensor = true;
    CO2ppm = gasSensor.getPPM();
  #endif  

  PacketCO2 Packet;

  Packet.Packet              = DATA_CO2ppm;
  Packet.UID                 = Settings.UID;
  Packet.IsNeedWriteDataBase = IsNeedWriteDataBase;
  Packet.IsSensor            = IsSensor;
  Packet.IsAlarm             = IsAlarm; 
  Packet.CO2ppm              = CO2ppm;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketPressure(bool IsNeedWriteDataBase = true, bool IsAlarm = false) 
{
  float Pressure = 0;
  bool IsSensor = false;
  
  #ifdef BME280Sensor
    IsSensor = true;
    Pressure    = bme.readPressure() / 133.3;
  #endif    

  PacketPressure Packet;

  Packet.Packet              = DATA_PRESSURE;
  Packet.UID                 = Settings.UID;
  Packet.IsNeedWriteDataBase = IsNeedWriteDataBase;
  Packet.IsSensor            = IsSensor;
  Packet.IsAlarm             = IsAlarm; 
  Packet.Pressure            = Pressure;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketStateAlert(uint32_t TypeSensor)
{
  bool Alert;
  #ifdef TEMPERATURE_SENSOR
    if(TypeSensor == 1) Alert = Settings.AlertTempAndHumIsOn;
  #endif
  #ifdef CO2_SENSOR
    if(TypeSensor == 2) Alert = Settings.AlertCO2IsOn;
  #endif
  #ifdef PRESSURE_SENSOR
    if(TypeSensor == 4) Alert = Settings.AlertPressureIsOn;
  #endif

  PacketDataAlert Packet;

  Packet.Packet              = STATE_ALERT;
  Packet.UID                 = Settings.UID;
  Packet.TypeSensor          = TypeSensor;
  Packet.AlertIsOn           = Alert;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

void SendPacketDataLimit(uint8_t TypeSensor, bool TypeLimit)
{
  int16_t LimitValue = 1;  
  #ifdef TEMPERATURE_SENSOR
    if(TypeSensor == 1) { 
      if (TypeLimit) LimitValue = MaxLimitT;
      else           LimitValue = MinLimitT;
    }
  #endif
  #ifdef CO2_SENSOR
    if(TypeSensor == 2) {
      if (TypeLimit) LimitValue = MaxLimitCO2;
    }
  #endif
  #ifdef PRESSURE_SENSOR
    if(TypeSensor == 4) { 
      delay(1);
    }
  #endif

  PacketDataLimit Packet;

  Packet.Packet              = DATA_LIMIT_VALUE;
  Packet.UID                 = Settings.UID;
  Packet.TypeSensor          = TypeSensor;
  Packet.LimitType           = TypeLimit;
  Packet.LimitValue          = LimitValue;

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
  #ifdef PRESSURE_SENSOR
    if(TypeSensor == 4) Timer = Settings.TimerPressure;
  #endif

  PacketTimerSensor Packet;

  Packet.Packet              = DATA_TIMER_SENSOR;
  Packet.UID                 = Settings.UID;
  Packet.TimerSensor         = Timer;

  uint16_t DataSize = sizeof(Packet);  // Размер структуры
  SendPacket((uint8_t*)&Packet, DataSize);
}

#ifdef CONTROLLER_LED
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

    #ifdef DETECTED_SENSOR
      Packet.ModeWork = Automode;
    #endif

    uint16_t DataSize = sizeof(Packet);  // Размер структуры
    SendPacket((uint8_t*)&Packet, DataSize);
  }
#endif

void SendPacketDeviceConfig()
{
  PacketDeviceConfig Packet;

  Packet.Packet     = DEVICE_CONFIG;
  Packet.UID        = Settings.UID;

  #ifdef CONTROLLER_TELEMETRY
    Packet.DeviceType         = TELEMETRY;
    Packet.IsTempAndHumSensor = IsTempAndHumSensor;
    Packet.IsCO2Sensor        = IsCO2Sensor;
    Packet.IsPressureSensor   = IsPressureSensor;
    Packet.IsInaSensor        = IsInaSensor;
  #endif
  #ifdef CONTROLLER_LED
    Packet.DeviceType       = LEDCONTROL;
    Packet.IsDetectedSensor = IsDetectedSensor;
    Packet.IsLightSensor    = IsLightSensor;
  #endif
  #ifdef PANELCONTROL
    Packet.DeviceType       = CONTROLPANEL;
  #endif

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
  #ifdef CONTROLLER_TELEMETRY
    Packet.DeviceType = TELEMETRY;
  #endif
  #ifdef CONTROLLER_LED
    Packet.DeviceType = LEDCONTROL;
  #endif
  #ifdef PANELCONTROL
    Packet.DeviceType = CONTROLPANEL;
  #endif

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

      case GET_PRESSURE:
      {
        Serial.println("Запрос Давления");
        SendPacketPressure(false);
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

          case GET_ALERT:
          {
            if (ReceivedPacket.CommandData) SendPacketStateAlert(ReceivedPacket.CommandData);
            break;
          }

          case SET_ALERT:
          {
            if (ReceivedPacket.CommandData) {
              uint32_t Value = ReceivedPacket.CommandData;
              uint8_t TypeSensor = (Value >> 24) & 0xFF;       // Выделяем старший байт тип сенсора
              bool AlertIsOn = (Value >> 16) & 0xFF;           // Выделяем следующий байт тревога включена или нет
              
              #ifdef TEMPERATURE_SENSOR
                if (TypeSensor == 1) {
                  TempAndHumAlert = AlertIsOn;
                  Settings.AlertTempAndHumIsOn = TempAndHumAlert;
                }
              #endif

              #ifdef CO2_SENSOR
                if (TypeSensor == 2) {
                  CO2Alert = AlertIsOn;
                  Settings.AlertCO2IsOn = CO2Alert;
                  Serial.println(CO2Alert);
                }
              #endif

              #ifdef PRESSURE_SENSOR
                if (TypeSensor == 4) {
                  PressureAlert = AlertIsOn;
                  Settings.AlertPressureIsOn = PressureAlert;
                }
              #endif

              WriteSettings();
            }
            break;
          }
          
          case GET_LIMIT:
          {
            if (ReceivedPacket.CommandData) {
              uint32_t Value = ReceivedPacket.CommandData;

              uint8_t TypeSensor = (Value >> 24) & 0xFF;       // Выделяем старший байт тип сенсора
              bool TypeLimit = (Value >> 16) & 0xFF;           // Выделяем следующий байт тип порога минимальный или максимальный
          
              SendPacketDataLimit(TypeSensor, TypeLimit);
            }
            break;
          }

          case SET_LIMIT:
          {
            if (ReceivedPacket.CommandData) {
              uint32_t Value = ReceivedPacket.CommandData;

              uint8_t TypeSensor = (Value >> 24) & 0xFF;       // Выделяем старший байт тип сенсора
              bool TypeLimit = (Value >> 16) & 0xFF;           // Выделяем следующий байт тип порога минимальный или максимальный
              int16_t LimitValue = Value & 0xFFFF;             // Извлекаем последние 16 бит (2 байта)             

              #ifdef TEMPERATURE_SENSOR
                if (TypeSensor == 1) {
                  if (TypeLimit) {
                    MaxLimitT = LimitValue;
                    Settings.MaxLimitT = MaxLimitT;
                  }
                  else {
                    MinLimitT = LimitValue;
                    Settings.MinLimitT = MinLimitT;
                  }
                  WriteSettings();
                }
              #endif  

              #ifdef CO2_SENSOR
                if (TypeSensor == 2) {
                  if (TypeLimit) {
                    MaxLimitCO2 = LimitValue;
                    Settings.MaxLimitCO2 = MaxLimitCO2;

                    Serial.println(Settings.MaxLimitCO2);
                  }
                  WriteSettings();
                }
              #endif  

              #ifdef PRESSURE_SENSOR
                if (TypeSensor == 4) {

                }
              #endif
            }
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

          #ifdef PRESSURE_SENSOR
            case SET_TIME_PRESSURE:
            {
              if (ReceivedPacket.CommandData) {
                TimerPressure = ReceivedPacket.CommandData;
                Settings.TimerPressure = TimerPressure;
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

          case SET_MODE_WORK:
          {
            uint8_t Mode = ReceivedPacket.CommandData;
            if (Mode == 1)  Automode = true;
            else            Automode = false;

            Settings.ModeWork = Automode;
            WriteSettings();
          }
        #endif
      }
      break;
    }
  }
  return;
}
