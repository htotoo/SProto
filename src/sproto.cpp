#include "sproto.h"


uint8_t SProto::XorBytes[] = SPROTO_ENCRYPTIONPARAM_XOR; //encryption stuff;
size_t SProto::XorByteCount = SPROTO_ENCRYPTIONPARAM_XOR_SIZE;

uint32_t SProto::CreateHeader(uint8_t* packet, uint16_t cmd, uint32_t dataLength, uint8_t  encryption, uint16_t srcAddr, uint16_t dstAddr)
{
  packet[SPROTO_HEADER_POS_STARTBYTE] = SPROTO_STARTBYTE;
  packet[SPROTO_HEADER_POS_PROTOVER] = SPROTO_VERSION;
  *(uint32_t*)(&packet[SPROTO_HEADER_POS_DATALENGTH]) = dataLength;
  *(uint16_t*)(&packet[SPROTO_HEADER_POS_SRCADDR]) = srcAddr;
  *(uint16_t*)(&packet[SPROTO_HEADER_POS_DSTADDR]) = dstAddr;
  packet[SPROTO_HEADER_POS_ENCRYPTION] = encryption;
  *(uint16_t*)(&packet[SPROTO_HEADER_POS_CMD]) = cmd;
  packet[SPROTO_HEADER_POS_CRC] = 0;
  SProto::UpdateHeaderCrc(packet);
  return SPROTO_HEADER_LENGTH; //used from the array, to help positioning
}

void SProto::UpdateHeaderDataSize(uint8_t* packet, uint32_t dataLength)
{
  packet[SPROTO_HEADER_POS_DATALENGTH] = dataLength;
  SProto::UpdateHeaderCrc(packet);
}

void SProto::UpdateHeaderEncryption(uint8_t* packet, uint8_t encryption)
{
  packet[SPROTO_HEADER_POS_ENCRYPTION] = encryption;
  SProto::UpdateHeaderCrc(packet);
}

void SProto::UpdateHeaderCrc(uint8_t* packet)
{
  packet[SPROTO_HEADER_POS_CRC] = SProto::CalcCrc8(packet, SPROTO_HEADER_LENGTH - 1);
}

uint16_t SProto::CalcCrc16(uint8_t* byteArrStart, uint32_t length, uint16_t initial) {
  uint16_t ret = initial;
  for (uint32_t i = 0; i < length; ++i)
  {
    ret += byteArrStart[i];
  }
  return ret;
}

uint8_t SProto::CalcCrc8(uint8_t* byteArrStart, uint32_t length, uint8_t initial) {
  uint8_t ret = initial;
  for (uint32_t i = 0; i < length; ++i)
  {
    ret += byteArrStart[i];
  }
  return ret;
}

uint32_t SProto::CalculateCommandDataLength(uint16_t cmd)
{
  switch (cmd)
  {
    case SPROTO_CMD_CMEAS:
      return SPROTO_LENGTH_UNKNOWN;
    case SPROTO_CMD_SMEAS:
      return 0;
    default:
      return SProto::CalculateCommandDataLength(cmd);
  }
}

uint32_t SProto::CalculateMeasurementDataLength(uint16_t dataType, bool withHeader)
{
  uint32_t ret = 0;
  if (withHeader) ret += SPROTO_MEASHEADER_LENGTH; //data header added
  switch (dataType)
  {
    case SPROTO_MEASID_NETID:
      ret += sizeof(SPM_NetId);
      break;
    case SPROTO_MEASID_TIME:
      ret += sizeof(SPM_Time);
      break;
    case SPROTO_MEASID_TEMPERATURE:
      ret += sizeof(SPM_Temperature);
      break;
    case SPROTO_MEASID_PRESSURE:
      ret += sizeof(SPM_Pressure);
      break;
    case SPROTO_MEASID_HUMIDITY:
      ret += sizeof(SPM_Humidity);
      break;
    case SPROTO_MEASID_WINDSPEED:
      ret += sizeof(SPM_WindSpeed);
      break;
    case SPROTO_MEASID_WINDDIR:
      ret += sizeof(SPM_WindDirection);
      break;
    case SPROTO_MEASID_REMAININGDATA:
      ret += sizeof(SPM_RemainingStoredData);
      break;
    case SPROTO_MEASID_ILLUMINANCE:
      ret += sizeof(SPM_Illuminance);
      break;
    case SPROTO_MEASID_GPSCOORD:
      ret += sizeof(SPM_GPSCOORD);
      break;
    case SPROTO_MEASID_GPSDETAILED:
      ret += sizeof(SPM_GPSDETAILED);
      break;
    case SPROTO_MEASID_RADSPECTRUM512:
      ret += sizeof(SPM_RadSpect512);
      break;
    case SPROTO_MEASID_RADSPECTRUM1024:
      ret += sizeof(SPM_RadSpect1024);
      break;
    case SPROTO_MEASID_RADSPECTRUM2048:
      ret += sizeof(SPM_RadSpect2048);
      break;
    case SPROTO_MEASID_RADSPECTRUM4096:
      ret += sizeof(SPM_RadSpect4096);
      break;
    case SPROTO_MEASID_AQI:
      ret += sizeof(SPM_AQI);
      break;
    case SPROTO_MEASID_GASPPM:
      ret += sizeof(SPM_GasPpm);
      break;
    case SPROTO_MEASID_VOLTAGE:
      ret += sizeof(SPM_Voltage);
      break;
    case SPROTO_MEASID_CURRENT:
      ret += sizeof(SPM_Current);
      break;
    case SPROTO_MEASID_POWER:
      ret += sizeof(SPM_Power);
      break;
    case SPROTO_MEASID_RGB:
      ret += sizeof(SPM_RGB);
      break;
    case SPROTO_MEASID_RGBW:
      ret += sizeof(SPM_RGBW);
      break;
    case SPROTO_MEASID_RGBWW:
      ret += sizeof(SPM_RGBWW);
      break;
    case SPROTO_MEASID_OPENCLOSE:
      ret += sizeof(SPM_OpenClose);
      break;
    case SPROTO_MEASID_ONOFF:
      ret += sizeof(SPM_OnOff);
      break;
    case SPROTO_MEASID_MOTION:
      ret += sizeof(SPM_Motion);
      break;
    case SPROTO_MEASID_VISIBILITYDIST:
      ret += sizeof(SPM_VisibilityDist);
      break;
    case SPROTO_MEASID_SMS:
      ret += sizeof(SPM_SMS);
      break;
    case SPROTO_MEASID_SIEVERT:
      ret += sizeof(SPM_Sievert);
      break;
    case SPROTO_MEASID_RADIATIONGRAY:
      ret += sizeof(SPM_RadiationGray);
      break;
    case SPROTO_MEASID_RADIATIONXRAY:
      ret += sizeof(SPM_RadiationXray);
      break;
    case SPROTO_MEASID_GEIGERCPS:
      ret += sizeof(SPM_GeigerCps);
      break;
    case SPROTO_MEASID_GEIGERCPM:
      ret += sizeof(SPM_GeigerCpm);
      break;
    case SPROTO_MEASID_UVINDEX:
      ret += sizeof(SPM_UVIndex);
      break;
    case SPROTO_MEASID_SOILMOISURE:
      ret += sizeof(SPM_SoilMoisure);
      break;
    case SPROTO_MEASID_DISTANCE:
      ret += sizeof(SPM_Distance);
      break;
    case SPROTO_MEASID_CARDID:
      ret += sizeof(SPM_CardId);
      break;
    case SPROTO_MEASID_RSSI:
      ret += sizeof(SPM_Rssi);
      break;
    case SPROTO_MEASID_SNR:
      ret += sizeof(SPM_Snr);
      break;
    case SPROTO_MEASID_INVALID:
      break;
    default:
      ret += CalculateMeasurementDataLengthImp(dataType);
  }
  return ret;
}

void SProto::MeasurementCreateHeader(uint8_t* dataPart, uint16_t dataType, uint8_t dataSerNum, uint8_t timeFrame)
{
  *(uint16_t*)&dataPart[SPROTO_MEASHEADER_POS_TYPE] = dataType;
  dataPart[SPROTO_MEASHEADER_POS_SERNUM] = dataSerNum;
  dataPart[SPROTO_MEASHEADER_POS_TIMEFRAME] = timeFrame;
}

bool SProto::IsValidHeader(uint8_t* packet)
{
  if (packet[SPROTO_HEADER_POS_STARTBYTE] != SPROTO_STARTBYTE) return false;
  if (packet[SPROTO_HEADER_POS_PROTOVER] != SPROTO_VERSION) return false;
  uint8_t crc = SProto::CalcCrc8(packet, SPROTO_HEADER_LENGTH - 1);
  if (packet[SPROTO_HEADER_POS_CRC] != crc) return false;
  return true;
}

uint32_t SProto::UpdateDataCrc(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet)) return 0;
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  *(uint16_t*)(&(packet[SPROTO_HEADER_LENGTH + dl])) = SProto::CalcCrc16(&packet[SPROTO_HEADER_LENGTH], dl);
  return SPROTO_HEADER_LENGTH + dl + 2;
}

bool SProto::IsValidData(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet)) return 0;
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  uint16_t crc = SProto::CalcCrc16(&packet[SPROTO_HEADER_LENGTH], dl);
  return (*(uint16_t*)(&(packet[SPROTO_HEADER_LENGTH + dl])) == crc);
}

bool SProto::IsValidPacket(uint8_t* packet)
{
  return (IsValidHeader(packet)); //also uses the header check
}

uint32_t SProto::GetFullPacketLength(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet)) return 0;
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  return SPROTO_HEADER_LENGTH + dl + 2;  //header + data + crc
}

void SProto::DecryptData(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet)) return;
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  uint8_t encryption = packet[SPROTO_HEADER_POS_ENCRYPTION];
  if (encryption == SPROTO_ENCRYPTION_NONE) return; //no need to encrypt
  if (encryption == SPROTO_ENCRYPTION_XOR) //simple xor
  {
    SProto::DecryptionXor(&packet[SPROTO_HEADER_LENGTH], dl);
    return;
  }
  SProto::DecryptDataImp(packet);
}

void SProto::EncryptData(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet)) return;
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  uint8_t encryption = packet[SPROTO_HEADER_POS_ENCRYPTION];
  if (encryption == SPROTO_ENCRYPTION_NONE) return; //no need to encrypt
  if (encryption == SPROTO_ENCRYPTION_XOR) //simple xor
  {
    SProto::EncryptionXor(&packet[SPROTO_HEADER_LENGTH], dl);
    return;
  }
  SProto::EncryptDataImp(packet);
}

void SProto::EncryptionXor(uint8_t* data, uint32_t length)
{
  for (uint32_t i = 0; i < length; ++i)
  {
    uint8_t xb = SProto::XorBytes[ i % SProto::XorByteCount];
    data[i] = data[i] ^ xb;
  }
}

uint16_t SProto::GetCmdFromPacket(uint8_t* packet)
{
  return *(uint16_t*)&packet[SPROTO_HEADER_POS_CMD];
}

bool SProto::MeasParseDataPart(uint8_t* packet, uint32_t offset, SPROTO_MEADHEADERSTRUCT* result)
{
  result->measTypeId = *(uint16_t*)&packet[offset + SPROTO_MEASHEADER_POS_TYPE];
  result->serial = packet[offset + SPROTO_MEASHEADER_POS_SERNUM];
  result->timeFrame = packet[offset + SPROTO_MEASHEADER_POS_TIMEFRAME];
  result->dataSizeWithHeader = SProto::CalculateMeasurementDataLength(result->measTypeId);
  return true;
}


#ifdef SPROTO_MQTTHELPER
uint8_t SProto::GetHADeviceTypeId(uint16_t dataType)
{
  if (dataType == SPROTO_MEASID_NETID) return SPROTO_MQTT_DEVTYPEID_UNKNOWN;
  if (dataType == SPROTO_MEASID_TIME) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_TEMPERATURE) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_PRESSURE) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_HUMIDITY) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_WINDSPEED) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_WINDDIR) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_ILLUMINANCE) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_GPSCOORD) return SPROTO_MQTT_DEVTYPEID_DEVTRACKER;
  if (dataType == SPROTO_MEASID_GPSDETAILED) return SPROTO_MQTT_DEVTYPEID_DEVTRACKER;
  if (dataType == SPROTO_MEASID_AQI) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_GASPPM) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_VOLTAGE) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_CURRENT) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_POWER) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_RGB) return SPROTO_MQTT_DEVTYPEID_LIGHT;
  if (dataType == SPROTO_MEASID_RGBW) return SPROTO_MQTT_DEVTYPEID_LIGHT;
  if (dataType == SPROTO_MEASID_RGBWW) return SPROTO_MQTT_DEVTYPEID_LIGHT;
  if (dataType == SPROTO_MEASID_OPENCLOSE) return SPROTO_MQTT_DEVTYPEID_BINARYSENSOR;
  if (dataType == SPROTO_MEASID_ONOFF) return SPROTO_MQTT_DEVTYPEID_BINARYSENSOR;
  if (dataType == SPROTO_MEASID_MOTION) return SPROTO_MQTT_DEVTYPEID_BINARYSENSOR;
  if (dataType == SPROTO_MEASID_VISIBILITYDIST) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_SMS) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_SIEVERT) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_GEIGERCPS) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_GEIGERCPM) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_RADIATIONGRAY) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_RADIATIONXRAY) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_UVINDEX) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_SOILMOISURE) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_DISTANCE) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_CARDID) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_RSSI) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  if (dataType == SPROTO_MEASID_SNR) return SPROTO_MQTT_DEVTYPEID_SENSOR;
  return SPROTO_MQTT_DEVTYPEID_UNKNOWN;
}

const char* SProto::GetDataTypeStr(uint16_t dataType)
{
  if (dataType == SPROTO_MEASID_NETID) return "netid";
  if (dataType == SPROTO_MEASID_TIME) return "time";
  if (dataType == SPROTO_MEASID_TEMPERATURE) return "temparature";
  if (dataType == SPROTO_MEASID_PRESSURE) return "pressure";
  if (dataType == SPROTO_MEASID_HUMIDITY) return "humidity";
  if (dataType == SPROTO_MEASID_WINDSPEED) return "windspeed";
  if (dataType == SPROTO_MEASID_WINDDIR) return "winddir";
  if (dataType == SPROTO_MEASID_ILLUMINANCE) return "illuminance";
  if (dataType == SPROTO_MEASID_GPSCOORD) return "gpscoord";
  if (dataType == SPROTO_MEASID_GPSDETAILED) return "gpsdetailed";
  if (dataType == SPROTO_MEASID_AQI) return "aqi";
  if (dataType == SPROTO_MEASID_GASPPM) return "gasppm";
  if (dataType == SPROTO_MEASID_VOLTAGE) return "voltage";
  if (dataType == SPROTO_MEASID_CURRENT) return "current";
  if (dataType == SPROTO_MEASID_POWER) return "power";
  if (dataType == SPROTO_MEASID_RGB) return "rgb";
  if (dataType == SPROTO_MEASID_RGBW) return "rgbw";
  if (dataType == SPROTO_MEASID_RGBWW) return "rgbww";
  if (dataType == SPROTO_MEASID_OPENCLOSE) return "openclose";
  if (dataType == SPROTO_MEASID_ONOFF) return "onoff";
  if (dataType == SPROTO_MEASID_MOTION) return "motion";
  if (dataType == SPROTO_MEASID_VISIBILITYDIST) return "visibilitydist";
  if (dataType == SPROTO_MEASID_SMS) return "sms";
  if (dataType == SPROTO_MEASID_SIEVERT) return "sievert";
  if (dataType == SPROTO_MEASID_GEIGERCPS) return "geigercps";
  if (dataType == SPROTO_MEASID_GEIGERCPM) return "geigercpm";
  if (dataType == SPROTO_MEASID_RADIATIONGRAY) return "radiogray";
  if (dataType == SPROTO_MEASID_RADIATIONXRAY) return "radioxray";
  if (dataType == SPROTO_MEASID_UVINDEX) return "uvindex";
  if (dataType == SPROTO_MEASID_SOILMOISURE) return "soilmoisure";
  if (dataType == SPROTO_MEASID_DISTANCE) return "distance";
  if (dataType == SPROTO_MEASID_CARDID) return "cardid";
  if (dataType == SPROTO_MEASID_RSSI) return "rssi";
  if (dataType == SPROTO_MEASID_SNR) return "snr";
  return "unk";
}
const char* SProto::GetDataTypeUnitStr(uint16_t dataType)
{
  if (dataType == SPROTO_MEASID_NETID) return "";
  if (dataType == SPROTO_MEASID_TIME) return "";
  if (dataType == SPROTO_MEASID_TEMPERATURE) return "°C";
  if (dataType == SPROTO_MEASID_PRESSURE) return "hPa";
  if (dataType == SPROTO_MEASID_HUMIDITY) return "%";
  if (dataType == SPROTO_MEASID_WINDSPEED) return "mps";
  if (dataType == SPROTO_MEASID_WINDDIR) return "°";
  if (dataType == SPROTO_MEASID_ILLUMINANCE) return "lx";
  if (dataType == SPROTO_MEASID_GPSCOORD) return "";
  if (dataType == SPROTO_MEASID_GPSDETAILED) return "";
  if (dataType == SPROTO_MEASID_AQI) return "AQI";
  if (dataType == SPROTO_MEASID_GASPPM) return "ppm";
  if (dataType == SPROTO_MEASID_VOLTAGE) return "V";
  if (dataType == SPROTO_MEASID_CURRENT) return "A";
  if (dataType == SPROTO_MEASID_POWER) return "W";
  if (dataType == SPROTO_MEASID_RGB) return "";
  if (dataType == SPROTO_MEASID_RGBW) return "";
  if (dataType == SPROTO_MEASID_RGBWW) return "";
  if (dataType == SPROTO_MEASID_OPENCLOSE) return "";
  if (dataType == SPROTO_MEASID_ONOFF) return "";
  if (dataType == SPROTO_MEASID_MOTION) return "";
  if (dataType == SPROTO_MEASID_VISIBILITYDIST) return "m";
  if (dataType == SPROTO_MEASID_SMS) return "";
  if (dataType == SPROTO_MEASID_SIEVERT) return "Sv";
  if (dataType == SPROTO_MEASID_GEIGERCPS) return "cps";
  if (dataType == SPROTO_MEASID_GEIGERCPM) return "cpm";
  if (dataType == SPROTO_MEASID_RADIATIONGRAY) return "Gy";
  if (dataType == SPROTO_MEASID_RADIATIONXRAY) return "Xy";
  if (dataType == SPROTO_MEASID_UVINDEX) return "UvI";
  if (dataType == SPROTO_MEASID_SOILMOISURE) return "%";
  if (dataType == SPROTO_MEASID_DISTANCE) return "cm";
  if (dataType == SPROTO_MEASID_CARDID) return "";
  if (dataType == SPROTO_MEASID_RSSI) return "dB";
  if (dataType == SPROTO_MEASID_SNR) return "dB";
  return "";
}

String SProto::GetMQTTDataTopic(const char* hostname, const char* mqttPrefix, uint16_t measTypeId, uint8_t dataSerNum, uint8_t timeFrame)
{
  String ret = String(mqttPrefix);
  ret = ret + String(hostname);
  ret = ret + String("/");
  ret = ret + String(SProto::GetDataTypeStr(measTypeId));
  ret = ret + String("-");
  ret = ret + String(dataSerNum);
  if (timeFrame > 0)
  {
    ret = ret + String("-");
    ret = ret + String(timeFrame);
  }
  ret = ret + String("/state");
  return ret;
}

String SProto::GetMQTTAttributesTopic(const char* hostname, const char* mqttPrefix, uint16_t measTypeId, uint8_t dataSerNum, uint8_t timeFrame)
{
  String ret = String(mqttPrefix);
  ret = ret + String(hostname);
  ret = ret + String("/");
  ret = ret + String(SProto::GetDataTypeStr(measTypeId));
  ret = ret + String("-");
  ret = ret + String(dataSerNum);
  if (timeFrame > 0)
  {
    ret = ret + String("-");
    ret = ret + String(timeFrame);
  }
  ret = ret + String("/attributes");
  return ret;
}


#endif


#ifdef SPROTO_DEBUG
void SProto::PrintDataHex(uint8_t* packet, uint32_t length)
{
  for (uint32_t j = 0; j < length; j++)
    printf("%02X ", packet[j]);
  printf("\n");
}

void SProto::PrintHeader(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet))
  {
    printf("Invalid header.\n");
    return;
  }
  printf("Proto version: %hhu\n", packet[SPROTO_HEADER_POS_PROTOVER]);
  printf("Data length  : %u\n", *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH]);
  uint16_t addr = *(uint16_t*)&packet[SPROTO_HEADER_POS_SRCADDR];
  if (addr == SPROTO_ADDR_NONE)
  {
    printf("Src addr not given.\n");
  }
  else
  {
    printf("Src addr : %hu\n", addr);
  }
  addr = *(uint16_t*)&packet[SPROTO_HEADER_POS_DSTADDR];
  if (addr == SPROTO_ADDR_NONE)
  {
    printf("Dst addr not given.\n");
  }
  else if (addr == SPROTO_ADDR_BROADCAST)
  {
    printf("Dst addr is broadcast.\n");
  }
  else
  {
    printf("Dst addr : %hu\n", addr);
  }
  uint8_t enc = packet[SPROTO_HEADER_POS_ENCRYPTION];
  if (enc == SPROTO_ENCRYPTION_NONE)
  {
    printf("Encryption is disabed\n");
  }
  else
  {
    printf("Encryption : %hhu\n", enc);
  }
  printf("Crc        : %hhu\n", packet[SPROTO_HEADER_POS_CRC]);
  //parse the command
  uint16_t cmd = *(uint16_t*)&packet[SPROTO_HEADER_POS_CMD];
  printf("Cmd        : %hu\n", cmd);
  if (cmd == SPROTO_CMD_CMEAS) printf("Cmd: C->S Measurement sending.\n");
  if (cmd == SPROTO_CMD_SMEAS) printf("Cmd: S->C Measurement request.\n");
}

void SProto::PrintMeasDataDetails(uint8_t* packet)
{
  if (!SProto::IsValidHeader(packet))
  {
    printf("Invalid header.\n");
    return;
  }
  uint16_t cmd = *(uint16_t*)&packet[SPROTO_HEADER_POS_CMD];
  if (cmd != SPROTO_CMD_CMEAS)
  {
    printf("Not a SPROTO_CMD_CMEAS command packet.\n");
    return;
  }
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  uint32_t offset = SPROTO_HEADER_LENGTH;
  dl += SPROTO_HEADER_LENGTH; //last data byte
  SPROTO_MEADHEADERSTRUCT dataHead;
  while (offset < dl)
  {
    SProto::MeasParseDataPart(packet, offset, &dataHead);
    printf("Data meas type: %hu\n", dataHead.measTypeId);
    if (dataHead.measTypeId == SPROTO_MEASID_NETID)
    {
      SPM_NetId tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Network ID: %02X %02X %02X %02X %02X %02X\n", tmp.x[0], tmp.x[1], tmp.x[2], tmp.x[3], tmp.x[4], tmp.x[5]);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_TIME)
    {
      SPM_Time tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Time: %lld\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_TEMPERATURE)
    {
      SPM_Temperature tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Temperature: %.2f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_PRESSURE)
    {
      SPM_Pressure tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Pressure: %.2f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_HUMIDITY)
    {
      SPM_Humidity tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Humidity: %.2f\n", tmp / 10.0);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_WINDSPEED)
    {
      SPM_WindSpeed tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Wind speed: %.2f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_WINDDIR)
    {
      SPM_WindDirection tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Wind direction: %hu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_ILLUMINANCE)
    {
      SPM_Illuminance tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Illuminance: %.2f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GPSCOORD)
    {
      SPM_GPSCOORD tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("GPS: Longitude: %.6f, Latituse: %.6f, Altitude: %.2f\n", tmp.longitude, tmp.latitude, tmp.altitude);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GPSDETAILED)
    {
      SPM_GPSDETAILED tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("GPS: Longitude: %.6f, Latituse: %.6f, Altitude: %.2f, Sat: %hhu, Speed: %.2f\n", tmp.longitude, tmp.latitude, tmp.altitude, tmp.satelittes, tmp.speed);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_AQI)
    {
      SPM_AQI tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("AQI: %hu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GASPPM)
    {
      SPM_GasPpm tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Gasppm: %.3f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_VOLTAGE)
    {
      SPM_Voltage tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Voltage: %.3f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_CURRENT)
    {
      SPM_Current tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Current: %.3f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_POWER)
    {
      SPM_Power tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Power: %.3f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_RGB)
    {
      SPM_RGB tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("RGB. R: %hhu G: %hhu, B: %hhu\n", tmp.r, tmp.g, tmp.b);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_RGBW)
    {
      SPM_RGBW tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("RGBW. R: %hhu G: %hhu, B: %hhu, W: %hhu\n", tmp.r, tmp.g, tmp.b, tmp.w);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_RGBWW)
    {
      SPM_RGBWW tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("RGBWW. R: %hhu G: %hhu, B: %hhu, W: %hhu, Warm: %hhu\n", tmp.r, tmp.g, tmp.b, tmp.w, tmp.warmw);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_OPENCLOSE)
    {
      SPM_OpenClose tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("OpenClose " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(tmp));
    }
    if (dataHead.measTypeId == SPROTO_MEASID_ONOFF)
    {
      SPM_OpenClose tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("OnOff: " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(tmp));
    }
    if (dataHead.measTypeId == SPROTO_MEASID_MOTION)
    {
      SPM_Motion tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Motion: " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(tmp));
    }
    if (dataHead.measTypeId == SPROTO_MEASID_VISIBILITYDIST)
    {
      SPM_VisibilityDist tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Visibility distance: %.3f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_SMS)
    {
      SPM_SMS tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("SMS: --todo parse --\n");
    }
    if (dataHead.measTypeId == SPROTO_MEASID_SIEVERT)
    {
      SPM_Sievert tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Sievert: %.10f\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GEIGERCPS)
    {
      SPM_GeigerCps tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("CPS: %lu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GEIGERCPM)
    {
      SPM_GeigerCpm tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("CPM: %lu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_RADIATIONGRAY)
    {
      SPM_RadiationGray tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Radio Gray: %lu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_RADIATIONXRAY)
    {
      SPM_RadiationXray tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Radio Xray: %lu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_UVINDEX)
    {
      SPM_UVIndex tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("UV Index: %hhu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_SOILMOISURE)
    {
      SPM_SoilMoisure tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("SoilMoisure: %hhu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_DISTANCE)
    {
      SPM_Distance tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Distance: %d\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_CARDID)
    {
      SPM_CardId tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("CardID. --todo \n");
    }
    if (dataHead.measTypeId == SPROTO_MEASID_RSSI)
    {
      SPM_Rssi tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("RSSI: %l\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_SNR)
    {
      SPM_Snr tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("SNR: %l\n", tmp);
    }

    if (dataHead.measTypeId == SPROTO_MEASID_RADSPECTRUM512 || dataHead.measTypeId == SPROTO_MEASID_RADSPECTRUM1024 || dataHead.measTypeId == SPROTO_MEASID_RADSPECTRUM2048 || dataHead.measTypeId == SPROTO_MEASID_RADSPECTRUM4096)
    {
      printf("Rad Spectrum. --todo \n");
    }
    if (dataHead.measTypeId == SPROTO_MEASID_REMAININGDATA)
    {
      SPM_RemainingStoredData tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      printf("Remaining stored data: %lu\n", tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_INVALID)
    {
      printf("Invalid data\n");
    }

    offset += dataHead.dataSizeWithHeader;
  }
}

#endif
