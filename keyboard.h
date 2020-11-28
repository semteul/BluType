#define PS2_BUF_LENGTH 10
class PS2Class { // ps2 객체 하나만 생성
  public:
    volatile bool isTX = false;   // PS2 신호 보내면 TX, 안보내면 RX
    volatile byte buf = 0;        // 버퍼
    volatile bool loaded = false; // 버퍼에 데이터가 있는지 확인
    
    volatile byte dataPIN = 3;    // 데이터 핀 (기본 3번)
    volatile byte clockPIN = 2;   // 클럭 핀   (기번 2번)
    
    void begin(byte cp, byte dp);
    void start();
    void stop();
    byte read();  // 읽어들이기
    void loop();
} PS2;

void PS2_interrupt();
