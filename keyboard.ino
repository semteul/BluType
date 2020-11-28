void PS2Class::begin(byte cp, byte dp) {
  clockPIN = cp;
  dataPIN = dp;
  
  start();
}

void PS2Class::start() {
  isTX = false;
  buf = 0;
  loaded = false;
  
  pinMode(dataPIN,INPUT_PULLUP);  // 핀 입력
  pinMode(clockPIN,INPUT_PULLUP); // 핀 입력
  
  attachInterrupt( digitalPinToInterrupt(clockPIN), PS2_interrupt, FALLING ); // 인터럽트
}

void PS2Class::stop() {
  detachInterrupt( digitalPinToInterrupt(clockPIN)); // 인터럽트 해제
}

// 읽어들이기
byte PS2Class::read() {
  if(loaded){
    loaded = false;
    return buf;
  }
  return 0;
}

void PS2Class::loop() {
  
}

void PS2_interrupt() {
  static unsigned long tm = 0;
  static byte bitCount = 0;
  static byte data = 0;
  
  if(PS2.isTX){  // 송신 모드
    
  } else {  // 수신 모드
    if( millis() - tm > 250 ){
      bitCount = 0;
      data = 0;
    }
    tm = millis();
  
    bitCount++;
    switch(bitCount){
      case 1:   // Start Bit
        break;
      case 2:   // Data 0
      case 3:   
      case 4:   
      case 5:   
      case 6:   
      case 7:   
      case 8:   
      case 9:   // Data 7
        data |= (digitalRead(KEYBOARD_DATA_PIN))<<(bitCount-2);
        break;
      case 10:   // Parity
        break;
      case 11:  // Stop bit
        PS2.buf = data;
        PS2.loaded = true;
      default:
        bitCount = 0;
        data = 0;
    }
  }
}
