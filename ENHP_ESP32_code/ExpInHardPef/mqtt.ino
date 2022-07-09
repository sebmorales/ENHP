
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting to: hardwaremovement.com");
  char userName[40] = "";
  eeprom_user.toCharArray(userName, sizeof(userName));

  while (!client.connect(userName)) {
//  while (!client.connect("esp")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("servo_" + eeprom_user);
  client.subscribe("out_" + eeprom_user);
  client.subscribe("buzzer_" + eeprom_user);
  client.subscribe("serial_" + eeprom_user);

}

void messageReceived(String &topic, String &payload) {
  //if you want to recieve other topics, make sure to subscribe beforehand^
  if (topic == "servo_" + eeprom_user) {
    servo.write(payload.toInt());
  }
  if (topic == "out_" + eeprom_user) {
    analogWrite(out, payload.toInt());
  }
  if (topic == "buzzer_" + eeprom_user) {
    analogWrite(buzzer, 50);
    delay(20);
    analogWrite(buzzer, 0);
  }
  if (topic == "serial_" + eeprom_user) {
    Serial.print(payload);
  }
}


void mqttSend() {
  Serial.println("sending mqtt");
  client.publish("tilt_" + eeprom_user, String(tilt_value));
  delay(100);
  client.publish("pot_" + eeprom_user, String(potentiometer_value));
  delay(100);
  client.publish("photo_" + eeprom_user, String(photoresistor_value));
  delay(100);

}
