Button::Button() {
  
}
void Button::begin(byte pin) {
  _ready = false;

  
  // 핀 세팅
  uint8_t port = digitalPinToPort(pin);
  _bitMask = digitalPinToBitMask(pin);
  
  _ptrModeReg = portModeRegister(port);
  _ptrOutReg = portOutputRegister(port);
  _ptrInReg = portInputRegister(port);
  
  *_ptrModeReg &= ~_bitMask; // INPUT 세팅
  *_ptrOutReg |= _bitMask;   // Port 레지스터 세팅 (PULL UP 저항 연결)

  
  _tick=0;
  _tm=0;
  _prevTickTm=0;
  _prevPinState = 0;

  
  _ready = true;
}
void Button::loop() {
  unsigned long interval;
  
  if(_tick == 0 && millis() - _tm > 100) { // 마지막 입력으로부터 100  밀리초 이하일 때
    if(! ((*_ptrInReg) & _bitMask) ){   // LOW(눌림)인 경우
      _tick = 1;
      _tm = millis();
      _prevTickTm = _tm;
      _prevPinState = 0;  // 눌린 상태
    }
    return;
  }
  
  interval = millis() - _tm;
  if(interval > 600){  // 처음 눌린지 n밀리초 이상이면 버튼 눌린 횟수 따라 처리 
    switch(_tick) {
      case 1: // 한번 클릭
      case 2: // 두번 클릭
      case 3: // 세번 클릭
        _data = _tick;
        break;
      default: // 그 외 입력은 에러 처리
        _data = 0;  
        break;
    }
    _tick = 0;
    _tm = millis();
    return;
  }

  // 핀 FALLING EDGE인 경우 _tick++, 오류 제거 위해 _prevTickTm 검사
  if(millis() - _prevTickTm > 150 && (_prevPinState & ~(*_ptrInReg) & _bitMask) ) {
    _tick++;
    _prevTickTm = millis(); // 오류 제외
  }
  _prevPinState = *_ptrInReg;
}

byte Button::read() {
  byte data = _data;
  _data = 0;
  return data;
}
