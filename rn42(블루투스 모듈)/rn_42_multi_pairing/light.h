/*
 * LED 점멸 기능
 */

#define LIGHT_OFF        0  // LED 상태, LightStatus에 대입해서 LED 제어
#define LIGHT_ON         1
#define LIGHT_BLINK      2
#define LIGHT_BLINK_FAST 3

// LED 핀 기본 설정 : 13번 (PORTB bit6)
#define LIGHTMASK B00100000 // LED 마스크
#define LIGHTPORT PORTB     // LED PORT 
#define LIGHTDDR DDRB       // LED DDR

#define LIGHT_INTERVAL      500 // LED 점멸 숫자
#define LIGHT_INTERVAL_FAST 100

byte LightCtrl = LIGHT_OFF; // LED 제어 전역변수

void lightBegin();
void lightLoop();
