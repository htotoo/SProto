
#include <sproto.h>

uint8_t packet[300];

#define MY_ADDR 0x0002
#define DEST_ADDR 0x0001
#define BROADCAST_ADDR 0xffff

#define HOSTNAME "EXAMPLE_HOST"
#define MQTTPREFIX "/root/data/"


void CreateDataPacket()
{
  SProto::CreateHeader(packet, SPROTO_CMD_CMEAS, 0 , SPROTO_ENCRYPTION_XOR,MY_ADDR,BROADCAST_ADDR);
  SPM_Temperature temp = 20.1; //C°
  SPM_Voltage volt = 3.65; //V
  SPM_GPSDETAILED tmp;
  tmp.latitude = 37.2513; //°
  tmp.longitude = -115.87404; //°
  tmp.altitude = 111; //m
  tmp.satelittes = 4; 
  tmp.speed = 0;
  tmp.hdop = 0;
  uint32_t len = SProto::SimpleMeasurementAdd(packet, SPROTO_MEASID_GPSDETAILED, &tmp);
  len = SProto::SimpleMeasurementAdd(packet, SPROTO_MEASID_TEMPERATURE, &temp);
  len = SProto::SimpleMeasurementAdd(packet, SPROTO_MEASID_VOLTAGE, &volt);
  Serial.print("Packet size: ");
  Serial.print(len);
  Serial.println(" bytes");

}

void ParseDataPacket()
{
  uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
  uint32_t offset = SPROTO_HEADER_LENGTH;
  dl += SPROTO_HEADER_LENGTH; //last data byte
  SPROTO_MEASHEADERSTRUCT dataHead;
  while (offset < dl)
  {
    SProto::MeasParseDataPart(packet, offset, &dataHead);
    String mqttTopic = SProto::GetMQTTDataTopic(HOSTNAME, MQTTPREFIX, dataHead.measTypeId, dataHead.serial, dataHead.timeFrame);
    Serial.print("MQTT Topic: "); Serial.println(mqttTopic);
    
    if (dataHead.measTypeId == SPROTO_MEASID_TEMPERATURE)
    {
      SPM_Temperature tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      Serial.print("Temperature: "); Serial.println(tmp);
      //SMqtt::SendMessage(mqttTopic.c_str(), String(tmp, 2).c_str());  //USER YOUR MQTT CLIENT TO SEND IT
    }
    if (dataHead.measTypeId == SPROTO_MEASID_PRESSURE)
    {
      SPM_Pressure tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      Serial.print("Pressure: "); Serial.println(tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_HUMIDITY)
    {
      SPM_Humidity tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      Serial.print("Humidity: "); Serial.println(tmp / 10.0);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_VOLTAGE)
    {
      SPM_Voltage tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      Serial.print("Voltage: "); Serial.println(tmp);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GPSCOORD)
    {
      SPM_GPSCOORD tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      Serial.print("GPS: Longitude: ");
      Serial.print(tmp.longitude, 6);
      Serial.print(", Latitude: ");
      Serial.print(tmp.latitude);
      Serial.print(", Altitude: ");
      Serial.println(tmp.altitude);
    }
    if (dataHead.measTypeId == SPROTO_MEASID_GPSDETAILED)
    {
      SPM_GPSDETAILED tmp;
      SProto::MeasGetDataPart(packet, offset, &tmp);
      Serial.print("GPS: Longitude: "); Serial.print(tmp.longitude, 6);
      Serial.print(", Latitude: ");
      Serial.print(tmp.latitude, 6);
      Serial.print(", Altitude: ");
      Serial.print(tmp.altitude);
      Serial.print(", Sats: ");
      Serial.print(tmp.satelittes);
      Serial.print(", Speed: ");
      Serial.println(tmp.speed, 2);
    }
    offset += dataHead.dataSizeWithHeader;
  }
}


void setup()
{
  Serial.begin(115200);
  delay(500);
  //Create a simple packet:
  CreateDataPacket();
  //Print debug info about it:
  SProto::PrintMeasDataDetails(packet);
  //Encrypt it
  SProto::EncryptData(packet);
  //Decrypt it
  SProto::DecryptData(packet);
  //check crc
  if (SProto::IsValidData(packet)) Serial.println("Packet is valid"); else Serial.println("Invalid packet!");
  
  //Parse and MQTT helper
  ParseDataPacket();

  //Create a packet with a Reboot CMD
  SProto::CreateHeader(packet, SPROTO_CMD_SRESTART, 0 , SPROTO_ENCRYPTION_XOR, MY_ADDR, DEST_ADDR);
  uint16_t cmd = SProto::GetCmdFromPacket(packet);
  if (cmd == SPROTO_CMD_SRESTART) Serial.println("Got restart cmd.");
}


void loop()
{
}
