/*
 * LED 출력
 * 
 * 모드 종류
 * 
 * begin(pin)                          : 핀 번호로 초기화
 * setLight(bool mode)                 : 켜기, 끄기 
 */

// 점멸 간격
#define LIGHT_BLINK_INTERVAL      600
#define LIGHT_BLINK_INTERVAL_FAST 300
// LED 모드
#define LIGHT_OFF        0  
#define LIGHT_ON         1
#define LIGHT_BLINK      2
#define LIGHT_BLINK_FAST 3

// LED
class Light {
  private:
    uint8_t _bitMask;               // 핀 비트마스크 
    volatile uint8_t * _ptrModeReg; // DDR 레지스터
    volatile uint8_t * _ptrOutReg;  // PORT 레지스터

    volatile bool _ready = false;   // 초기화되었는지 체크
    
    byte _mode = LIGHT_OFF;         // 기본 모드
    unsigned long _tm=0;            // 버튼 처리 시작시간
    unsigned long _prevTickTm=0;    // 버튼 눌림간 시간
  public:
    Light();
    void begin(byte pin);
    void set(byte mode);
    void loop();
};
