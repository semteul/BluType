/* 
 * begin으로 초기화, loop문에 loop() 삽입
 * 버튼 눌릴 경우 600밀리초 간 최대 3번 연속 눌림까지 체크
 * read()로 입력 읽음. 1,2,3 반환 -> n번 연속으로 눌림 / 0 반환 -> 입력 없음.
*/
class Button {
  private:
    uint8_t _bitMask;               // 핀 비트마스크 
    volatile uint8_t * _ptrModeReg; // DDR 레지스터
    volatile uint8_t * _ptrOutReg;   // PORT 레지스터
    volatile uint8_t * _ptrInReg;   // PIN 레지스터

    volatile bool _ready = false; // 초기화 체크
    
    byte _tick = 0;
    byte _data = 0; 
    uint8_t _prevPinState = 0;
    
    unsigned long _tm=0;          // 버튼 처리 시작시간
    unsigned long _prevTickTm=0;  // 버튼 눌림간 시간
  public:
    Button();
    void begin(byte pin);
    void loop();
    byte read();
};
