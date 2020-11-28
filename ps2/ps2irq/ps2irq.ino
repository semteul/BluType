/*
 * ps/2 프로토콜 입력 체크
 */
#define KEYBOARD_DATA_PIN 3
#define KEYBOARD_CLK_PIN  2

/*
 * 전송과정
if(!KeyboardSending) break;
KeyboardSendData = 0xFF;
KeyboardSending = true; 
*/

void kbInterruptRX() {
  static unsigned long tm = 0;
  static byte bitCount = 0;
  static byte data = 0;
  static byte parity = 0;
  
  if( millis() - tm > 250 ) {
    bitCount = 0;
    data = 0;
  }
  tm = millis();

  bitCount++;
  switch(bitCount){
    case 1:   // Start Bit
      break;
    case 2:   // Data 0
    case 3:   // Data 1
    case 4:   // Data 2
    case 5:   // Data 3
    case 6:   // Data 4
    case 7:   // Data 5
    case 8:   // Data 6
    case 9:   // Data 7
      data |= (digitalRead(KEYBOARD_DATA_PIN))<<(bitCount-2);
      parity ++;
      break;
    case 10:   // Parity
      
      break;
    case 11:  // Stop bit
      Serial.println(data,HEX);
    default:
      bitCount = 0;
      data = 0;
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(KEYBOARD_CLK_PIN, INPUT_PULLUP);
  pinMode(KEYBOARD_DATA_PIN, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt( KEYBOARD_CLK_PIN ), kbInterruptRX, FALLING );
}

void loop()
{
  
}
