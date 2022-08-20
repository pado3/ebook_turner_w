// ebook_turner_w.py BLE電子書籍めくり機 for Android with ESP32
// copyright (c) by @pado3
// ver.1.0 2022/08/19
// 
// Note:
//  ReaderはDOWNで送りUPで戻る。Kinoppyは逆。読書尚友・なろうリーダは選択可能でReaderに合わせる。
//  UARTはクロス接続。書込のお作法はこちら https://ht-deko.com/arduino/esp-wroom-32.html#04_02

#include <etw.h>  // Modify device name from <BleKeyboard.h>

BleKeyboard bleKeyboard;
int DN = 5;
int UP = 17;
int BK = 13;
int LED = 15; // GPIO1-3 is not recommended
 
void setup() {
  pinMode(LED,OUTPUT); 
  pinMode(DN,INPUT_PULLUP);
  pinMode(UP,INPUT_PULLUP);
  pinMode(BK,INPUT_PULLUP);
  Serial.begin(115200);
  bleKeyboard.begin();
  Serial.println();
  Serial.println(F("START " __FILE__ " from " __DATE__ " " __TIME__));
}
 
void loop() {
  if(!digitalRead(DN)) {  // push DOWN port
    digitalWrite(LED,HIGH);
    Serial.print("DOWN ");
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    delay(50);
    digitalWrite(LED,LOW);
    delay(400);
  }
  if(!digitalRead(UP)) {  // push UP port
    digitalWrite(LED,HIGH);
    Serial.print("UP ");
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    delay(50);
    digitalWrite(LED,LOW);
    delay(50);
    digitalWrite(LED,HIGH);
    delay(50);
    digitalWrite(LED,LOW);
    delay(300);
  }
  if(!digitalRead(BK)) {  // push BACK port
    digitalWrite(LED,HIGH);
    Serial.println("BK ");
    bleKeyboard.write(KEY_MEDIA_WWW_BACK);
    delay(900);
    digitalWrite(LED,LOW);
    delay(50);
  }
  delay(50);
}
