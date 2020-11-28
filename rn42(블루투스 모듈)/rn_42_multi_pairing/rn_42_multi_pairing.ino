/*
    현재 개발중인 멀티 페어링 소스코드
    완성 안됨
*/
#include "light.h"
#include "bluetooth.h"

void setup()
{

  buttonBegin(); // 버튼 초기화
  lightBegin();    // LED 초기화

  Serial.begin(9600);
  btBegin();
}

void loop()
{
  buttonLoop();  // 버튼 처리
  lightLoop();      // LED 처리

  btLoop(); // RN-42 처리
}
