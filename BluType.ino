/*
 * BluType
 * 작성자 : 김민석 (Minseok Kim) semteul32@gmail.com
 * 유선 키보드 -> 블루투스 HID 무선 키보드 변환 장치
 * 필요 장치 : 아두이노 보드(우노, 프로 미니, 마이크로 등등), RN-42(블루투스 HID), USB 커넥터 등
 * 자세한 내용은 아래 링크로
 * 
 * 
 * 테스트 환경 : 아두이노 우노 / 아두이노 메가 + rn-41 (펌웨어 버전 1.65)
 * 아두이노 우노에서 사용하려면 BLUETOOTH Serial1 -> BLUETOOTH Serial
 * 
 * RN-42 기본 설정
 * 
 */
#include "button.h"
#include "light.h"

#include "keycode.h"
#include "keytable.h"

#include "keyboard.h"
#include "BTmodule.h"

#define KEYBOARD_CLK_PIN  2
#define KEYBOARD_DATA_PIN 3

#define BUTTON_PIN 8
#define LIGHT_PIN 13

#define BLUETOOTH Serial1             // UNO에서 사용시 Serial1 -> Serial로 변경
#define SERIAL_DEBUGGING 1            // MEGA보드 이 외에는 시리얼 포트가 하나밖에 없어서 1에서 0으로 설정
#define BLUETOOTH_BAUDRATE 115200     // 블루투스 모듈과 통신 속도

Button button;
Light light;

void setup() {
  Serial.begin(9600);
  BLUETOOTH.begin(BLUETOOTH_BAUDRATE);
  
  button.begin(BUTTON_PIN);
  light.begin(LIGHT_PIN);
  PS2.begin(KEYBOARD_CLK_PIN,KEYBOARD_DATA_PIN);
}

void loop() {
  button.loop();
  light.loop();
  PS2.loop();
  BTmodule.loop();

  uint8_t data;
  if(data = PS2.read()) {
     BTmodule.send(BTmodule.translate_PS2(data));
  }

  // 연결하면 불켜짐, 연결 안되면 점멸
  if(BTmodule.isConnected){
    light.set(LIGHT_ON);
  } else {
    light.set(LIGHT_BLINK_FAST);
  }
  
  switch(button.read()){
    case 1: // 버튼 1번 누르면 RE connect
      PS2.stop();
      BTmodule.connect();
      PS2.start();
      break;
    case 2: // 버튼 2번 누르면 Disconnect
      PS2.stop();
      BTmodule.disconnect();
      break;
  }
}
