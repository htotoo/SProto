//HEADER SPECIFIC

#define SPROTO_HEADER_POS_STARTBYTE 0
#define SPROTO_HEADER_POS_PROTOVER 1
#define SPROTO_HEADER_POS_DATALENGTH 2
#define SPROTO_HEADER_POS_SRCADDR 6
#define SPROTO_HEADER_POS_DSTADDR 8
#define SPROTO_HEADER_POS_ENCRYPTION 10
#define SPROTO_HEADER_POS_CMD 11
#define SPROTO_HEADER_POS_CRC 13

#define SPROTO_HEADER_LENGTH 14
#define SPROTO_LENGTH_UNKNOWN 0xffffffff

#define SPROTO_STARTBYTE '&'
#define SPROTO_VERSION 0x01
#define SPROTO_ADDR_NONE 0x0000
#define SPROTO_ADDR_BROADCAST 0xffff

//COMMANDS

//client sends measurement datas to server
#define SPROTO_CMD_CMEAS 0x8001
//server requests the current measurement datas (newest)
#define SPROTO_CMD_SMEAS 0x0001
//server requests the current measurement datas (oldest)
#define SPROTO_CMD_SMEASHIST 0x0002
//server requests the stations name
#define SPROTO_CMD_SGETSTATIONNAME 0x0003
//server requests the client to reboot
#define SPROTO_CMD_SRESTART 0xfffe


//MEASUREMENT DATA SPECIFIC
#define SPROTO_MEASHEADER_LENGTH 4
#define SPROTO_MEASHEADER_POS_TYPE 0
#define SPROTO_MEASHEADER_POS_SERNUM 2
#define SPROTO_MEASHEADER_POS_TIMEFRAME 3
//DATA TYPE IDs

#define SPROTO_MEASID_NETID 0
#define SPROTO_MEASID_TIME 1
#define SPROTO_MEASID_TEMPERATURE 2
#define SPROTO_MEASID_PRESSURE 3
#define SPROTO_MEASID_HUMIDITY 4
#define SPROTO_MEASID_WINDSPEED 5
#define SPROTO_MEASID_WINDDIR 6
#define SPROTO_MEASID_ILLUMINANCE 7
#define SPROTO_MEASID_GPSCOORD 8
#define SPROTO_MEASID_GPSDETAILED 9
#define SPROTO_MEASID_AQI 10
#define SPROTO_MEASID_GASPPM 11
#define SPROTO_MEASID_VOLTAGE 12
#define SPROTO_MEASID_CURRENT 13
#define SPROTO_MEASID_POWER 14
#define SPROTO_MEASID_RGB 15
#define SPROTO_MEASID_RGBW 16
#define SPROTO_MEASID_RGBWW 17
#define SPROTO_MEASID_OPENCLOSE 18
#define SPROTO_MEASID_ONOFF 19
#define SPROTO_MEASID_MOTION 20
#define SPROTO_MEASID_VISIBILITYDIST 21
#define SPROTO_MEASID_SMS 22
#define SPROTO_MEASID_SIEVERT 23
#define SPROTO_MEASID_GEIGERCPS 24
#define SPROTO_MEASID_GEIGERCPM 25
#define SPROTO_MEASID_RADIATIONGRAY 26
#define SPROTO_MEASID_RADIATIONXRAY 27
#define SPROTO_MEASID_UVINDEX 28
#define SPROTO_MEASID_SOILMOISURE 29
#define SPROTO_MEASID_DISTANCE 30
#define SPROTO_MEASID_CARDID 31
#define SPROTO_MEASID_RSSI 32
#define SPROTO_MEASID_SNR 33

#define SPROTO_MEASID_RADSPECTRUM512 100
#define SPROTO_MEASID_RADSPECTRUM1024 101
#define SPROTO_MEASID_RADSPECTRUM2048 102
#define SPROTO_MEASID_RADSPECTRUM4096 103

#define SPROTO_MEASID_STAIONNAME 0xfffd
#define SPROTO_MEASID_REMAININGDATA 0xfffe
#define SPROTO_MEASID_INVALID 0xffff

//DATA FORMATS

struct SPM_StationName
{
  char x[16]; //max 15 char
};
struct SPM_NetId {
  uint8_t x[6];
}; //6 byte network id
typedef uint64_t SPM_Time;  //8 byte unix time
typedef float SPM_Temperature; //celsius
typedef float SPM_Pressure; //hpa
typedef uint16_t SPM_Humidity; // % *10
typedef float SPM_WindSpeed; // m/s
typedef uint16_t SPM_WindDirection;  // 0-360�
typedef float SPM_Illuminance; //LX
typedef uint16_t SPM_RemainingStoredData;  //number of stored data packets
typedef SPM_RemainingStoredData SPM_RemData;  // -||-
typedef uint16_t SPM_AQI; //Air quality index 0-500
typedef float SPM_GasPpm;  //Gas sensor value in ppm
typedef float SPM_Voltage; //Voltage
typedef float SPM_Current; //Ampers
typedef float SPM_Power;  //Watts
struct SPM_RGB
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
struct SPM_RGBW
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t w;
};
struct SPM_RGBWW
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t w;
  uint8_t warmw;
};
typedef uint8_t SPM_OpenClose;  //Open / close sensor state. 1 - 8 numbers.
typedef uint8_t SPM_OnOff; //On / off state. 1-8 numbers.
typedef uint8_t SPM_Motion; //Motion on / off state. 1-8 numbers.
typedef float SPM_VisibilityDist; //Visibility distance in kilometers. (eg weather forecast)
struct SPM_SMS
{
  char message[160];
  char sender[20];
  char destination[20];
  char time[30];
};
typedef float SPM_Sievert; //radiation sievert
typedef uint32_t SPM_GeigerCps; //radiation in cps
typedef uint32_t SPM_GeigerCpm; //radiation in cpm
typedef uint32_t SPM_RadiationGray; //radiation in gray
typedef uint32_t SPM_RadiationXray; //radiation xray
typedef uint8_t SPM_UVIndex; //uv index
typedef uint8_t SPM_SoilMoisure; //soil moisure
typedef int32_t SPM_Distance; //distance. eg in cm, for ultrasonic distance meters
typedef int16_t SPM_Rssi; //dB
typedef float SPM_Snr; //dB 
struct SPM_CardId
{
  char x[20]; //card id. (rfid cards)
};
struct SPM_GPSCOORD //gps data, just the coordinates and height
{
  float latitude;
  float longitude;
  float altitude;
};
struct SPM_GPSDETAILED //all important gps data
{
  float latitude;
  float longitude;
  float altitude;
  uint8_t satelittes;
  float speed;
  float hdop;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};
struct SPM_RadSpect512 {
  uint32_t ch[512];
};  //radiology spectrum stored in 512 channels
struct SPM_RadSpect1024 {
  uint32_t ch[1024];
}; //radiology spectrum stored in 1024 channels
struct SPM_RadSpect2048 {
  uint32_t ch[2048];
}; //radiology spectrum stored in 2048 channels
struct SPM_RadSpect4096 {
  uint32_t ch[4096];
}; //radiology spectrum stored in 4096 channels

//DATA TIME INTERVALS
#define SPROTO_TIME_INSTANTANEOUS  0
#define SPROTO_TIME_INSTANT 0

#define SPROTO_TIME_AVG_1S 1
#define SPROTO_TIME_AVG_5S 2
#define SPROTO_TIME_AVG_15S 3
#define SPROTO_TIME_AVG_30S 4
#define SPROTO_TIME_AVG_1M 5
#define SPROTO_TIME_AVG_5M 6
#define SPROTO_TIME_AVG_10M 7
#define SPROTO_TIME_AVG_15M 8
#define SPROTO_TIME_AVG_30M 9
#define SPROTO_TIME_AVG_1H 10
#define SPROTO_TIME_AVG_1p5H 11
#define SPROTO_TIME_AVG_2H 12
#define SPROTO_TIME_AVG_3H 13
#define SPROTO_TIME_AVG_4H 14
#define SPROTO_TIME_AVG_8H 15
#define SPROTO_TIME_AVG_12H 16
#define SPROTO_TIME_AVG_1D 17
#define SPROTO_TIME_AVG_2D 18
#define SPROTO_TIME_AVG_3D 19
#define SPROTO_TIME_AVG_5D 20
#define SPROTO_TIME_AVG_7D 21
#define SPROTO_TIME_AVG_14D 22
#define SPROTO_TIME_AVG_30D 23
#define SPROTO_TIME_AVG_60D 24
#define SPROTO_TIME_AVG_90D 25
#define SPROTO_TIME_AVG_120D 26
#define SPROTO_TIME_AVG_180D 27
#define SPROTO_TIME_AVG_1Y 28
#define SPROTO_TIME_AVG_2Y 29
#define SPROTO_TIME_AVG_5Y 30
#define SPROTO_TIME_AVG_10Y 31
#define SPROTO_TIME_AVG_15Y 32
#define SPROTO_TIME_AVG_30Y 33
#define SPROTO_TIME_AVG_50Y 34
#define SPROTO_TIME_AVG_100Y 35

#define SPROTO_TIME_MIN_1S 41
#define SPROTO_TIME_MIN_5S 42
#define SPROTO_TIME_MIN_15S 43
#define SPROTO_TIME_MIN_30S 44
#define SPROTO_TIME_MIN_1M 45
#define SPROTO_TIME_MIN_5M 46
#define SPROTO_TIME_MIN_10M 47
#define SPROTO_TIME_MIN_15M 48
#define SPROTO_TIME_MIN_30M 49
#define SPROTO_TIME_MIN_1H 50
#define SPROTO_TIME_MIN_1p5H 51
#define SPROTO_TIME_MIN_2H 52
#define SPROTO_TIME_MIN_3H 53
#define SPROTO_TIME_MIN_4H 54
#define SPROTO_TIME_MIN_8H 55
#define SPROTO_TIME_MIN_12H 56
#define SPROTO_TIME_MIN_1D 57
#define SPROTO_TIME_MIN_2D 58
#define SPROTO_TIME_MIN_3D 59
#define SPROTO_TIME_MIN_5D 60
#define SPROTO_TIME_MIN_7D 61
#define SPROTO_TIME_MIN_14D 62
#define SPROTO_TIME_MIN_30D 63
#define SPROTO_TIME_MIN_60D 64
#define SPROTO_TIME_MIN_90D 65
#define SPROTO_TIME_MIN_120D 66
#define SPROTO_TIME_MIN_180D 67
#define SPROTO_TIME_MIN_1Y 68
#define SPROTO_TIME_MIN_2Y 69
#define SPROTO_TIME_MIN_5Y 70
#define SPROTO_TIME_MIN_10Y 71
#define SPROTO_TIME_MIN_15Y 72
#define SPROTO_TIME_MIN_30Y 73
#define SPROTO_TIME_MIN_50Y 74
#define SPROTO_TIME_MIN_100Y 75

#define SPROTO_TIME_MAX_1S 81
#define SPROTO_TIME_MAX_5S 82
#define SPROTO_TIME_MAX_15S 83
#define SPROTO_TIME_MAX_30S 84
#define SPROTO_TIME_MAX_1M 85
#define SPROTO_TIME_MAX_5M 86
#define SPROTO_TIME_MAX_10M 87
#define SPROTO_TIME_MAX_15M 88
#define SPROTO_TIME_MAX_30M 89
#define SPROTO_TIME_MAX_1H 90
#define SPROTO_TIME_MAX_1p5H 91
#define SPROTO_TIME_MAX_2H 92
#define SPROTO_TIME_MAX_3H 93
#define SPROTO_TIME_MAX_4H 94
#define SPROTO_TIME_MAX_8H 95
#define SPROTO_TIME_MAX_12H 96
#define SPROTO_TIME_MAX_1D 97
#define SPROTO_TIME_MAX_2D 98
#define SPROTO_TIME_MAX_3D 99
#define SPROTO_TIME_MAX_5D 100
#define SPROTO_TIME_MAX_7D 101
#define SPROTO_TIME_MAX_14D 102
#define SPROTO_TIME_MAX_30D 103
#define SPROTO_TIME_MAX_60D 104
#define SPROTO_TIME_MAX_90D 105
#define SPROTO_TIME_MAX_120D 106
#define SPROTO_TIME_MAX_180D 107
#define SPROTO_TIME_MAX_1Y 108
#define SPROTO_TIME_MAX_2Y 109
#define SPROTO_TIME_MAX_5Y 110
#define SPROTO_TIME_MAX_10Y 111
#define SPROTO_TIME_MAX_15Y 112
#define SPROTO_TIME_MAX_30Y 113
#define SPROTO_TIME_MAX_50Y 114
#define SPROTO_TIME_MAX_100Y 115

#define SPROTO_TIME_SUM_1S 120
#define SPROTO_TIME_SUM_5S 121
#define SPROTO_TIME_SUM_15S 122
#define SPROTO_TIME_SUM_30S 123
#define SPROTO_TIME_SUM_1M 124
#define SPROTO_TIME_SUM_5M 125
#define SPROTO_TIME_SUM_10M 126
#define SPROTO_TIME_SUM_15M 127
#define SPROTO_TIME_SUM_30M 128
#define SPROTO_TIME_SUM_1H 129
#define SPROTO_TIME_SUM_1p5H 130
#define SPROTO_TIME_SUM_2H 131
#define SPROTO_TIME_SUM_3H 132
#define SPROTO_TIME_SUM_4H 133
#define SPROTO_TIME_SUM_8H 134
#define SPROTO_TIME_SUM_12H 135
#define SPROTO_TIME_SUM_1D 136
#define SPROTO_TIME_SUM_2D 137
#define SPROTO_TIME_SUM_3D 138
#define SPROTO_TIME_SUM_5D 139
#define SPROTO_TIME_SUM_7D 140
#define SPROTO_TIME_SUM_14D 141
#define SPROTO_TIME_SUM_30D 142
#define SPROTO_TIME_SUM_60D 143
#define SPROTO_TIME_SUM_90D 144
#define SPROTO_TIME_SUM_120D 145
#define SPROTO_TIME_SUM_180D 146
#define SPROTO_TIME_SUM_1Y 147
#define SPROTO_TIME_SUM_2Y 148
#define SPROTO_TIME_SUM_5Y 149
#define SPROTO_TIME_SUM_10Y 150
#define SPROTO_TIME_SUM_15Y 151
#define SPROTO_TIME_SUM_30Y 152
#define SPROTO_TIME_SUM_50Y 153
#define SPROTO_TIME_SUM_100Y 154

#define SPROTO_TIME_ANY 255

//Encryption defaults.
#define SPROTO_ENCRYPTION_NONE 0x00
#define SPROTO_ENCRYPTION_XOR 0x01

#ifndef SPROTO_ENCRYPTIONPARAM_XOR
    #define SPROTO_ENCRYPTIONPARAM_XOR { 0xaa , 0xab, 0xbb, 0xba }
#endif
#ifndef SPROTO_ENCRYPTIONPARAM_XOR_SIZE
    #define SPROTO_ENCRYPTIONPARAM_XOR_SIZE 4
#endif
struct SPROTO_MEASHEADERSTRUCT
{
  uint16_t measTypeId;
  uint8_t serial;
  uint8_t timeFrame;
  uint32_t dataSizeWithHeader;
};


//MQTT Helpers
#define SPROTO_MQTT_DEVTYPEID_UNKNOWN 0
#define SPROTO_MQTT_DEVTYPEID_SENSOR 1
#define SPROTO_MQTT_DEVTYPEID_DEVTRACKER 2
#define SPROTO_MQTT_DEVTYPEID_LIGHT 3
#define SPROTO_MQTT_DEVTYPEID_BINARYSENSOR 4
