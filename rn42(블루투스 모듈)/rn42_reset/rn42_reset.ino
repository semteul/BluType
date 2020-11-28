                                 /*
 * RN42 모듈 리셋하기
 * https://dattasaurabh82.wordpress.com/2014/11/01/resetting-bluetooth-module/
 * 
 * 1. RN42 모듈의 GPIO4(RN42 보드 뒷면에 있는 PIO4핀)을 아두이노 13번 포트에 물린다.
 * 2. 아두이노의 LED가 켜져있을 떄 RN42의 전원을 켠다. (5v를 연결한다.)
 * 3. 세번 아두이노의 LED가 깜박이면 RN42가 빠르게 세번 깜빡인다.(리셋되었다는 뜻)
 * 4. 리셋이 완료되면 GPIO4를 아두이노와 연결해제한다.
 */
void setup() {
  pinMode(7, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
