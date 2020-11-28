#include <SoftwareSerial.h>
#include <EEPROM.h>   // EEPROM에 페어링 MAC 주소 저장

#define BT_MODE_NORMAL  0 // 일반 모드
#define BT_MODE_WAITING 1 // 페어링 모드
#define BT_MODE_CHANGE_REMOTE  2 // 기기 전환 모드

byte BtMode = BT_MODE_NORMAL;

#define BT_STATUS_STRING_WAITING    0 // Status String 기다리는중
#define BT_STATUS_STRING_RECEIVING  1 // Status String 입력 받는중
#define BT_STATUS_STRING_CONNECTED  2 // 입력 처리
#define BT_STATUS_STRING_GETADDRESS 3 // 주소 입력 받는중

byte BtConnected = false;

#define BT_REMOTES_NUMBER 2         // 디바이스 개수

byte BtRemoteID = 0;            // 연결된 디바이스 id
char BtRemoteAddress[13] = "";  // 페어링된 디바이스 주소

SoftwareSerial bluetooth(5, 4);

void btBegin();
void btLoop();


void btStatusCheck(char buf); 
void btGetAddress(char buf);

void btMode();

void btConnect();
void btDisconnect(); 
