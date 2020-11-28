void lightBegin() {
  LIGHTDDR |= LIGHTMASK;  // LED핀 출력으로 설정
}

void lightLoop() {
  static unsigned int tm=0;
  
  switch(LightCtrl){
    case LIGHT_OFF:
      LIGHTPORT &= ~LIGHTMASK;  // LED off
      break;
      
    case LIGHT_ON:
      LIGHTPORT |= LIGHTMASK; // LED on
      break;
      
    case LIGHT_BLINK:
      if(millis() - tm > LIGHT_INTERVAL){
       LIGHTPORT ^= LIGHTMASK; // LED 토글
       tm = millis();
      }
      break;
      
    case LIGHT_BLINK_FAST:
      if(millis() - tm > LIGHT_INTERVAL_FAST){
       LIGHTPORT ^= LIGHTMASK; // LED 토글
       tm = millis();
      }
      break;
  }
}
