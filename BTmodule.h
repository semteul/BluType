/*
 * 블루투스 모듈과 통신 담당
 */
// 데이터 송신 모드
#define SENDMODE_SEND             0 // 스캔코드
#define SENDMODE_BREAK            1 // 스캔코드 break
#define SENDMODE_SEND_AND_BREAK   2 // 스캔코드 송신 후 break

class BTmoduleClass {
  private:
    uint8_t HIDscancode[6] = {0x00,};       // HID 키 송신 데이터
    uint8_t HIDmodifier = 0b00000000;       // HID modifier 송신 데이터

    byte sendMode;                          // 데이터 송신 모드 지정 (스캔코드,스캔코드 break, 스캔코드 송신 후 break)

    bool addScancode(uint8_t data);         // HIDscancode에 데이터 추가
    bool deleteScancode(uint8_t data);      // HIDscancode에 데이터 제거
    void clearScancode();                   // HIDscancode 비움
    
    void bluetoothSend();                   // Modifier, HIDscancode 송신
    
  public:
    bool isConnected = false;
    void begin();                           // 초기화
    void loop();                            // 루프 (주로 블루투스 모듈 상태 감지)
    void connect();                         // 마지막 연결했던 디바이스와 재연결
    void disconnect();                      // 디바이스와 연결 끊기
    uint8_t translate_PS2(uint8_t ps2Code); // PS2 키코드를 HID 스캔코드로 번역
    void send(uint8_t data);                // HID 스캔코드를 Modifier 등 변환하여 전송
} BTmodule;
