/*
 * 버튼 처리
 * 버튼 두가지 모드
 * 1. 그냥 눌렀을 때 : 장치 변환
 * 2. 길게 1초 눌렀을 때 : 장치 연결 해제
 */
#define BTNPIN 12 // 버튼 핀

void buttonBegin() {
  pinMode(BTNPIN,INPUT_PULLUP); // 풀업저항 연결
}

void buttonLoop() {
  static unsigned long tm = 0;
  static unsigned long interval = 0;
  static bool bussy = false;
  
  if(tm == 0) {
    if(digitalRead(BTNPIN) == LOW) {  
      tm = millis();
    }
  } else {
    interval = millis() - tm;
    
    if(digitalRead(BTNPIN) == LOW) {
      if(!bussy && interval>=1000){  // 버튼 1초 이상 누른 경우 장치 연결 해제 & 페어링 대기
        bussy = true;
        BtMode = BT_MODE_WAITING;
      }
      
    } else {
      if(100<interval && interval<1000) {  // 다음 기기로 변경
        BtMode = BT_MODE_CHANGE_REMOTE;
      }
      tm = 0;
      bussy = false;
    }
  }
}
