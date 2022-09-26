// battery level check and feedback via BLE
uint8_t measBatteryLevel() {
  uint32_t Raw = adc1_get_raw(ADac); // raw data, same as analogRead()
  uint32_t Vcal;  // calibrated readout voltage [mV]
  uint8_t batt;  // battery level [%]
  esp_adc_cal_get_voltage(ADch, &adcChar, &Vcal);
  Serial.print("AD1 readout: " + String(Raw));
  Serial.println("/4095 , calibrated "+ String(Vcal) + "[mV]");
  // calculate percentage of battery in uint8_t
  if(Vmax < Vcal) {
    batt = 100;
  } else if(Vcal < Vth) {
    batt = 0;
  } else {
    batt = (Vcal - Vth)*100/(Vmax - Vth); // cast, use 0-255
  }
  Serial.print("battery level: ");
  Serial.print(batt);
  Serial.println("% (% means 3.0~3.3V)");
  return batt;
}

void checkbatt() {
  uint8_t batt = measBatteryLevel();
  bleKeyboard.setBatteryLevel(batt);
  if(batt = 0) {   // battery level LOW
    for(uint8_t i=0; i<5; i++) {   // long blink 5 times
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED, LOW);
      delay(1000);
    }
  }
}
