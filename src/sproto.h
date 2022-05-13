
#ifndef __SPROTO_H
#define __SPROTO_H

#include "../config.h"
#include <stddef.h>
#include <stdint.h>
#ifdef SPROTO_DEBUG
#include <stdio.h>
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)    \
  (((i) & 0x80ll) ? '1' : '0'), \
  (((i) & 0x40ll) ? '1' : '0'), \
  (((i) & 0x20ll) ? '1' : '0'), \
  (((i) & 0x10ll) ? '1' : '0'), \
  (((i) & 0x08ll) ? '1' : '0'), \
  (((i) & 0x04ll) ? '1' : '0'), \
  (((i) & 0x02ll) ? '1' : '0'), \
  (((i) & 0x01ll) ? '1' : '0')
#endif
#ifdef SPROTO_MQTTHELPER
#include <Arduino.h>
#endif
#include "sprotodefines.h"

class SProto
{
  public:
    //packet creation
    static uint32_t CreateHeader(uint8_t* packet, uint16_t cmd, uint32_t dataLength = 0, uint8_t  encryption = SPROTO_ENCRYPTION_NONE, uint16_t srcAddr = SPROTO_ADDR_NONE, uint16_t dstAddr = SPROTO_ADDR_BROADCAST);
    static uint32_t CreateHeaderMini(uint8_t* packet, uint16_t cmd, uint32_t dataLength = 0);
    static void UpdateHeaderDataSize(uint8_t* packet, uint32_t dataLength);
    static void UpdateHeaderEncryption(uint8_t* packet, uint8_t encryption);
    static void UpdateHeaderCrc(uint8_t* packet);
    static uint32_t GetHeaderLength(uint8_t* packet);

    static uint32_t CalculateCommandDataLength(uint16_t cmd);
    static uint32_t CalculateMeasurementDataLength(uint16_t dataType, bool withHeader = true);

    static void MeasurementCreateHeader(uint8_t* dataPart, uint16_t dataType, uint8_t dataSerNum = 0, uint8_t timeFrame = SPROTO_TIME_INSTANT);
    template <typename  T>
    static uint32_t MeasurementPutData(uint8_t* dataPart, T* data) {
      *(T*)&dataPart[SPROTO_MEASHEADER_LENGTH] = *data;
      return (sizeof(T) + SPROTO_MEASHEADER_LENGTH);
    }
    template <typename  T>
    static uint32_t MeasAddHeaderAndData(uint8_t* dataPart, uint16_t dataType, T* data, uint8_t dataSerNum = 0, uint8_t timeFrame = SPROTO_TIME_INSTANT)
    {
      SProto::MeasurementCreateHeader(dataPart, dataType, dataSerNum, timeFrame);
      return MeasurementPutData(dataPart, data);
    }
    template <typename  T>
    static uint32_t SimpleMeasurementAdd(uint8_t* packet, uint16_t dataType, T* data, uint8_t dataSerNum = 0, uint8_t timeFrame = SPROTO_TIME_INSTANT)
    {
      if (!SProto::IsValidHeader(packet)) return 0; //header presents?
      if (SProto::GetCmdFromPacket(packet) != SPROTO_CMD_CMEAS) return 0; //meas packet?
      uint32_t dl = *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH];
      uint32_t newdl = dl + MeasAddHeaderAndData(&packet[GetHeaderLength(packet) + dl], dataType, data, dataSerNum, timeFrame);
      SProto::UpdateHeaderDataSize(packet, newdl);
      return SProto::UpdateDataCrc(packet);
    }

    static void EncryptData(uint8_t* packet, bool updateDataCrc = true);
    static uint32_t UpdateDataCrc(uint8_t* packet);

    //packet parse
    static bool IsValidHeader(uint8_t* packet);
    static bool IsValidData(uint8_t* packet);
    static bool IsValidPacket(uint8_t* packet);
    static uint32_t GetFullPacketLength(uint8_t* packet);
    static uint32_t GetDataLength(uint8_t* packet) { return  *(uint32_t*)&packet[SPROTO_HEADER_POS_DATALENGTH]; };
    static void DecryptData(uint8_t* packet, bool updateHeaderEncrypt = true, bool updateDataCrc = false);
    static uint16_t GetAddrDestination(uint8_t* packet);
    static uint16_t GetAddrSource(uint8_t* packet);
    static bool MeasParseDataPart(uint8_t* packet, uint32_t offset, SPROTO_MEASHEADERSTRUCT* result);
    template <typename  T>
    static uint32_t MeasGetDataPart(uint8_t* packet, uint32_t offset, T* result)
    {
      *result = *(T*)&packet[SPROTO_MEASHEADER_LENGTH + offset];
      return offset + SPROTO_MEASHEADER_LENGTH + sizeof(T);
    }

    //common helper functions
    static uint16_t GetCmdFromPacket(uint8_t* packet);
    static uint16_t CalcCrc16(uint8_t* byteArrStart, uint32_t length, uint16_t initial = 0);
    static uint8_t CalcCrc8(uint8_t* byteArrStart, uint32_t length, uint8_t initial = 0);
    static void EncryptionXor(uint8_t* data, uint32_t length);
    static void DecryptionXor(uint8_t* data, uint32_t length) {
      SProto::EncryptionXor(data, length);  //since it is the same
    }
    static void HelpCopyStationName(SPM_StationName* dest, char* from, size_t len);
    //config
    static uint8_t XorBytes[]; //xor encryption byte array;
    static size_t XorByteCount; //xor encryption byte array element count
#ifdef SPROTO_MQTTHELPER
    static uint8_t GetHADeviceTypeId(uint16_t dataType);
    static const char* GetHADevStringByHaDevType(uint8_t devType);
    static const char* GetDataTypeStr(uint16_t dataType);
    static const char* GetDataTypeUnitStr(uint16_t dataType);
    static String GetMQTTDataTopic(const char* hostname, const char* stationId, const char* mqttPrefix, uint16_t measTypeId, uint8_t dataSerNum, uint8_t timeFrame = SPROTO_TIME_INSTANT);
    static String GetMQTTAttributesTopic(const char* hostname, const char* stationId,  const char* mqttPrefix, uint16_t measTypeId, uint8_t dataSerNum, uint8_t timeFrame = SPROTO_TIME_INSTANT);
#endif
    //debug functions
#ifdef SPROTO_DEBUG
    static void PrintDataHex(uint8_t* packet, uint32_t length);
    static void PrintHeader(uint8_t* packet);
    static void PrintMeasDataDetails(uint8_t* packet);
#endif
  protected:
    static uint32_t CalculateCommandDataLengthImp(uint16_t cmd) {
      return 0;  //to override by customisations
    }
    static uint32_t CalculateMeasurementDataLengthImp(uint16_t dataType) {
      return 0;  //to override by custimisations. DON'T include header size in it!
    }

    static void EncryptDataImp(uint8_t* packet) {}; //to override by custimisations.
    static void DecryptDataImp(uint8_t* packet) {}; //to override by custimisations.
};


#endif
