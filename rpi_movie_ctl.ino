// rpi_movie_ctl.ino Firefox movie controller on Raspberry Pi 5
// copyright (c) by @pado3
// ver.1.0 2025/02/19 Change key functions for watching rugby
//
// based upon ebook_turner_w.ino BLE電子書籍めくり機 for Android with ESP32
// ver.1.0 2022/08/19
// ver.2.0 2022/09/26 Add battery check function, refactoring
// ver.2.1 2022/10/01 Change voltage monitor position from reg. to batt.
// ver.2.2 2022/10/14 Correct LOW batt LED and some comment
//
// Note:
//  adc_gpio_init()はESP32 by Espressif 2.0.2が必要
//  アプリ切り換えスイッチはReaderに固定する
//  UARTはクロス接続。書込のお作法は： https://ht-deko.com/arduino/esp-wroom-32.html#04_02

#include <BleKeyboard.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

// define complicated magic words
#define ADin 35                // GPIO No., set Vdd/2 on board
#define ADun ADC_UNIT_1        // Unit No.
#define ADch ADC_CHANNEL_7     // Channel No. (NOT ADC1_CHANNEL_7)
#define ADat ADC_ATTEN_DB_11   // Attenuation in dB
#define ADac ADC1_CHANNEL_7    // Attenuation channel (NOT unit but channel !)
#define ADwi ADC_WIDTH_BIT_12  // 12bit: 4096step
#define ADof 1100              // default offset: 1100mV

esp_adc_cal_characteristics_t adcChar;               // Calibration data
BleKeyboard bleKeyboard("eBook_turner_w", "pado3");  // device name, manufacturer

uint8_t DN = 5;
uint8_t UP = 17;
uint8_t BK = 13;
uint8_t LED = 15;  // GPIO1-3 is not recommended
// Vmax and Vth will compare with Vcal, uint32_t
uint32_t Vmax = 1850;  // Li-Po max. 3.7/2=1.85V, in [mV]
uint32_t Vth = 1600;   // (Vdd min 3.0V + Reg. drop 0.1V + Margin 0.1V)/2, in [mV]

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(DN, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(BK, INPUT_PULLUP);
  // initialize ADC
  adc_gpio_init(ADun, ADch);              // initialize
  adc1_config_width(ADwi);                // resolution 12bit
  adc1_config_channel_atten(ADac, ADat);  // 11dB attenuation
  // push CAL information to adcChar
  esp_adc_cal_characterize(ADun, ADat, ADwi, ADof, &adcChar);
  Serial.begin(115200);
  Serial.println();
  Serial.println("START " __FILE__ " from " __DATE__ " " __TIME__);
  Serial.println("ver.2.2 2022/10/14 Correct LOW batt LED and some comment");
  checkbatt();
  bleKeyboard.begin();
  for (uint8_t i = 0; i < 3; i++) {  // splash, short blink 3 times
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}

void loop() {
  if (!digitalRead(DN)) {  // push DOWN port; Reader-Left
    digitalWrite(LED, HIGH);
    Serial.print("DOWN=Reader-Left ");
    bleKeyboard.write(KEY_LEFT_ARROW);
    checkbatt();
    delay(50);
    digitalWrite(LED, LOW);
    delay(400);
  }
  if (!digitalRead(UP)) {  // push UP port; Reader-Right
    digitalWrite(LED, HIGH);
    Serial.print("UP=Reader-Right ");
    bleKeyboard.write(KEY_RIGHT_ARROW);
    checkbatt();
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(300);
  }
  if (!digitalRead(BK)) {  // push BACK port; short:Escape, long:open Firefox
    digitalWrite(LED, HIGH);
    Serial.print("BK ");
    delay(500);
    if (!digitalRead(BK)) {  // long press
      Serial.print("LONG=Open Firefox; ");
      Serial.print("COMMAND ");
      bleKeyboard.write(KEY_LEFT_GUI);
      delay(300);
      Serial.print("DOWN ");
      bleKeyboard.write(KEY_DOWN_ARROW);
      delay(500);
      Serial.print("DOWN ");
      bleKeyboard.write(KEY_DOWN_ARROW);
      delay(500);
      Serial.print("RIGHT ");
      bleKeyboard.write(KEY_RIGHT_ARROW);
      delay(500);
      Serial.print("DOWN ");
      bleKeyboard.write(KEY_DOWN_ARROW);
      delay(500);
      Serial.print("ENTER ");
      bleKeyboard.write(KEY_RETURN);
    } else {  // Escape
      Serial.print("SHORT=Escape ");
      bleKeyboard.write(KEY_ESC);
    }
    checkbatt();
    delay(900);
    digitalWrite(LED, LOW);
    delay(50);
  }
  delay(50);
  /* めくり機のコード
  if(!digitalRead(DN)) {  // push DOWN port
    digitalWrite(LED,HIGH);
    Serial.print("DOWN ");
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    checkbatt();
    delay(50);
    digitalWrite(LED,LOW);
    delay(400);
  }
  if(!digitalRead(UP)) {  // push UP port
    digitalWrite(LED,HIGH);
    Serial.print("UP ");
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    checkbatt();
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
    Serial.print("BK ");
    bleKeyboard.write(KEY_MEDIA_WWW_BACK);
    checkbatt();
    delay(900);
    digitalWrite(LED,LOW);
    delay(50);
  }
  delay(50);
  */
}
