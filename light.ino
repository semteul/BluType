Light::Light() {
  
}
void Light::begin(byte pin){
  _ready = false;

  // 핀 세팅
  uint8_t port = digitalPinToPort(pin);
  _bitMask = digitalPinToBitMask(pin);
  _ptrModeReg = portModeRegister(port);
  _ptrOutReg = portOutputRegister(port);
  *_ptrModeReg |= _bitMask; // OUTPUT 세팅
  *_ptrOutReg &= ~_bitMask;  // OFF 세팅
  
  // 모드 세팅
  _mode = LIGHT_OFF;
  _tm=0;
  _prevTickTm=0;

  _ready = true;
}
void Light::loop() {
  if(!_ready) return; // 초기화중에는 set 불가능
  switch(_mode) {
    case LIGHT_BLINK:
      if(millis() - _tm > LIGHT_BLINK_INTERVAL){
        *_ptrOutReg ^= _bitMask;
        _tm = millis();
      }
      break;
    case LIGHT_BLINK_FAST:
      if(millis() - _tm > LIGHT_BLINK_INTERVAL_FAST){
        *_ptrOutReg ^= _bitMask;
        _tm = millis();
      }
      break;
  }
}
void Light::set(byte mode) {
  if(!_ready) return; // 초기화중에는 set 불가능
  _mode = mode;
  switch(_mode) {
    case LIGHT_OFF:
      *_ptrOutReg |= ~_bitMask; // OFF
      break;
    case LIGHT_ON:
      *_ptrOutReg &= _bitMask;  // ON
      break;
  }
}
