void btBegin() 
{
  bluetooth.begin(115200); // RN-42 기본 속도 115200bps
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$");
  delay(100); // 잠시 대기
  
  bluetooth.println("U,9600,N");  // 임시적으로 속도를 9600으로 변경. SoftwareSerial로는 115200속도가 너무 빠름
  bluetooth.begin(9600);
}

void btLoop()
{
  char buf;
  if(bluetooth.available())
  {
    buf = (char)bluetooth.read();
    Serial.print(buf);
    btStatusCheck(buf); // Status 체크
  }
  if(Serial.available())
  {
    bluetooth.print((char)Serial.read());
  } 
}

void btMode(char buf) {
  // BluType 모드
  static unsigned long tm = 0; // 타이머
  switch(BtMode)
  {
    case BT_MODE_WAITING:     // 페어링 대기 모드
      
      tm = 0;
      BtMode = BT_MODE_NORMAL;
      break;
      
    case BT_MODE_CHANGE_REMOTE:    // 기기 변경 모드
      
      tm = 0;
      BtMode = BT_MODE_NORMAL;
      break;
  }   
}

void btStatusCheck(char buf) {
  static byte index = 0;        // 문자열 index
  static byte flag = BT_STATUS_STRING_WAITING;     // Status String 플래그(상태 저장) 
  static unsigned long tm = 0;
  
  switch(flag) {
    case BT_STATUS_STRING_WAITING:
      if(buf == '%'){  // % 입력받으면 Status String 입력 시퀀스 시작
        flag = BT_STATUS_STRING_RECEIVING;
        index = 0;
      }
      break;
      
    case BT_STATUS_STRING_RECEIVING:  //  입력받는중
      // TODO : 에러 대비 문자열 유사도 체크
      switch(buf){
        case 'C': // %CONNECTED
          flag = BT_STATUS_STRING_CONNECTED;
          break;
        case 'D': // %DISCONNECTED
          BtConnected = false;
          flag = BT_STATUS_STRING_WAITING;
          break;
      }
      break;
    case BT_STATUS_STRING_CONNECTED:
      /*
      if(tm == 0){
        tm = millis();
        bluetooth.write((byte)0x00);  // 커맨드 모드로 들어가기 위해 연결 해제
        break;
      }
      if(millis() - tm <= 100) break; // 연결 해제
      Serial.print("**GF!!!**");
      bluetooth.print("$$$");
      bluetooth.print("GF\n");
      tm = 0;
      */
      flag = BT_STATUS_STRING_GETADDRESS;
      break;
    case BT_STATUS_STRING_GETADDRESS:
      /*
      if(tm == 0 ) {
        tm = millis();
        index = 0;
        break;
      } else if( millis() - tm <= 500 ) {
        if(index<12){
          BtRemoteAddress[index++] = buf;
          break;
        } else {
          BtRemoteAddress[12] = '\0';
        }
      }
      */
      BtConnected = true;
      tm = 0;
      flag = BT_STATUS_STRING_WAITING;
      break;
  }
}

void btConnect(String address) 
{
  bluetooth.print("$$$\n"); // Command 모드 진입
  bluetooth.print("C,");
  bluetooth.print(address);
  bluetooth.print("\n");
  bluetooth.print("---\n"); // Command 모드 종료
}

void btDisconnect() {
  // 0x00 : disconnect code
  if(BtConnected)
    bluetooth.write((byte)0x00);
}
