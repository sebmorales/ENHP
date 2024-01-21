#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  1200        /* Time ESP32 will go to sleep (in seconds)1200=20min */

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex


void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}



void initSleep() {
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();

  //wake up if you press the button
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); //1 = High, 0 = Low

  // wake up if enough time has passed
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

//see if the live performance slider is enabled, if it isn't, go to sleep
void checkPerformanceMode() {
  if (digitalRead(performance) == HIGH) {
    Serial.print(F("going to sleep to save battery"));
    checkInputs(true);//force the input broadcast
    delay(1000);
    Serial.flush();
    esp_deep_sleep_start();
  }

}
