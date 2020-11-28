/*
 * RN42 (HID 블루투스 모듈) 테스트
 * 우노보드, 기본 TX5, RX4
 */

#include <SoftwareSerial.h>

#define TXPIN 4
#define RXPIN 5

//SoftwareSerial bluetooth(TXPIN, RXPIN); // 블루투스 모듈과 통신
#define bluetooth Serial1

void setup()
{
  delay(500);
  
  Serial.begin(9600);

  
  bluetooth.begin(115200);

  // 9600으로 속도 하향 필요시 다음 소스 사용
  /*
  // $를 각각 세번식 입력하면 커맨드 모드로 들어간다.
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$");
  delay(100); // 잠시 대기
  
  bluetooth.println("U,9600,N");  // 임시적으로 속도를 9600으로 변경. SoftwareSerial로는 115200속도가 너무 빠름
  
  bluetooth.begin(9600);
  */
}

void loop()
{
  if(bluetooth.available())
  {
    Serial.print((char)bluetooth.read());
  }
  if(Serial.available())
  {
    bluetooth.print((char)Serial.read());
  } 
}
