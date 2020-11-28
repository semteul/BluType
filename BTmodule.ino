void BTmoduleClass::begin() {
#if SERIAL_DEBUGGING
  Serial.begin(9600);
  BLUETOOTH.begin(BLUETOOTH_BAUDRATE);
#endif
}
bool BTmoduleClass::addScancode(uint8_t data) {
 for(byte i=0;i<6;i++){
   if(HIDscancode[i] == 0x00) {
     HIDscancode[i] = data;
     return true;
   }
   if(HIDscancode[i] == data) {
     return false;
   }
 }
 return false;
}
bool BTmoduleClass::deleteScancode(uint8_t data) {
 for(byte i=0;i<6;i++){
   if(HIDscancode[i] == data) {
     HIDscancode[i] = 0x00;
     return true;
   }
 }
 return false;
}
void BTmoduleClass::clearScancode() {                  
 for(byte i=0;i<6;i++)
   HIDscancode[i] = 0x00;
}
void BTmoduleClass::bluetoothSend(){
 
#if SERIAL_DEBUGGING
 // 모듈 통신 테스트     
 Serial.print(HIDmodifier,HEX);
 for(byte i=0;i<6;i++){
   Serial.print(" ");
   Serial.print(HIDscancode[i],HEX);
 }
 Serial.println(" ");
#endif
 
 
 BLUETOOTH.write((byte)0xFD);
 BLUETOOTH.write((byte)0x09);
 BLUETOOTH.write((byte)0x01);
 
 BLUETOOTH.write(HIDmodifier); // modifier
 BLUETOOTH.write((byte)0x00);
 
 BLUETOOTH.write(HIDscancode[0]);
 BLUETOOTH.write(HIDscancode[1]);
 BLUETOOTH.write(HIDscancode[2]);
 BLUETOOTH.write(HIDscancode[3]);
 BLUETOOTH.write(HIDscancode[4]);
 BLUETOOTH.write(HIDscancode[5]);
}

void BTmoduleClass::loop() {
  static bool isReceivingStatus = false;  // 상태 데이터 받는중인지 체크
  char data;
  
  // 현재 블루투스 모듈 상태 입력받기
  if(BLUETOOTH.available()) {
    data = (char)BLUETOOTH.read();
    if(data == '%'){
      isReceivingStatus = true;
      return;
    }
    
    if(isReceivingStatus){
      
      if(data == 'C'){    // Connected
        isConnected = true;
#if SERIAL_DEBUGGING
        Serial.println("Connected");
#endif
      } else if(data == 'D') {    // Disconnected
        isConnected = false;
#if SERIAL_DEBUGGING
        Serial.println("Disconnected");
#endif
      }
      isReceivingStatus = false;
    }
  }
}

void BTmoduleClass::connect() {
  // 초기화
  HIDmodifier = 0;
  clearScancode();
  bluetoothSend();
  
  BLUETOOTH.write(0x00);  // 연결 끊기
  delay(10);
  BLUETOOTH.print("$$$"); // Command Mode
  delay(10);
  BLUETOOTH.print("CR\n"); // 다시 연결 시도
}
void BTmoduleClass::disconnect() {
  // 초기화
  HIDmodifier = 0;
  clearScancode();
  bluetoothSend();

  BLUETOOTH.write((byte)0x00);  // 연결 종료
}

uint8_t BTmoduleClass::translate_PS2(uint8_t ps2Code) { 
  static bool isBreak = false;
  static bool isExtend = false;
  static byte ignoreCount = 0;
  
  if(ignoreCount>0){  // 바이트 무시해야 할 경우
    ignoreCount--;
    return 0;
  }

  switch(ps2Code) {
    case 0xF0:        // Break
      isBreak = true;
      return 0;
      
    case 0xE0:        // Extend Key
      isExtend = true;
      return 0;
      
    case 0xE1:        // pause Key
      ignoreCount = 7;
      sendMode = SENDMODE_SEND_AND_BREAK;
      return HID_PAUSE;
      
    case 0xF1:        // 한자 Key
      sendMode = SENDMODE_SEND_AND_BREAK;
      return HID_CONTROL_RIGHT;
      
    case 0xF2:        // 한영 Key
      sendMode = SENDMODE_SEND_AND_BREAK;
      return HID_ALT_RIGHT;
  }
  
  // 변환
  if(isBreak){
    isBreak = false;
    sendMode = SENDMODE_BREAK;
    if(isExtend) {
      isExtend = false;
      if(ps2Code == 0x7C){ // Print Screen UP
        ignoreCount=3;
        return 0x46;
      }
      // Extend key UP
      if(ps2Code < sizeof(Translate_Table_Extends)/sizeof(uint8_t))
        return pgm_read_byte(&Translate_Table_Extends[ps2Code]);
      else
        return 0;
    }
    // Single key UP
    if(ps2Code < sizeof(Translate_Table)/sizeof(uint8_t))
      return pgm_read_byte(&Translate_Table[ps2Code]);
    else 
      return 0;
  }
  
  sendMode = SENDMODE_SEND;
  if(isExtend) {
    isExtend = false;
    if (ps2Code == 0x12) { // Print Screen DOWN
      ignoreCount=2;
      return 0x46;
    }
    // Extend key DOWN
    if(ps2Code < sizeof(Translate_Table_Extends)/sizeof(uint8_t))
      return pgm_read_byte(&Translate_Table_Extends[ps2Code]); 
    else
      return 0;
  }
  
  // Single key DOWN
  if(ps2Code < sizeof(Translate_Table)/sizeof(uint8_t))
    return pgm_read_byte(&Translate_Table[ps2Code]); 
  else
    return 0;
}

void BTmoduleClass::send(uint8_t data) {       // 현재 버퍼의 데이터를 블루투스에 송신
  uint8_t modifierMask = 0;
  if(data==0){  // data 0일 경우 출력 안함
    return;
  }
  if(data>=0xE0){ // Modifier 처리
     modifierMask = 1<<data-0xE0;
     switch(sendMode){
      case SENDMODE_SEND:
        HIDmodifier |= modifierMask;
        bluetoothSend();
        break;
      case SENDMODE_SEND_AND_BREAK:
        HIDmodifier |= modifierMask;
        bluetoothSend();
      case SENDMODE_BREAK:
        HIDmodifier &= ~modifierMask;
        bluetoothSend();
        break;
     }
  } else {
    switch(sendMode){
      case SENDMODE_SEND:
        addScancode(data);
        bluetoothSend();
        break;
      case SENDMODE_SEND_AND_BREAK:
        addScancode(data);
        bluetoothSend();
      case SENDMODE_BREAK:
        deleteScancode(data);
        bluetoothSend();
        break;
    }
  }
}
