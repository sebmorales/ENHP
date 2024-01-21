
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
  // client.subscribe(eeprom_user+"_send");
  Serial.println("\nconnected!");
  String topic= eeprom_user+"_msg";
  client.subscribe(topic.c_str());
  delay(500);

}

void messageReceived(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  //de-normalize values
  servo.write(int(float(doc["servo"])*180));
  analogWrite(relay,int(doc["relay"])*255);
  analogWrite(out,int(float(doc["out"])*255));
  analogWrite(io18,int(float(doc["io18"]))*255);
  // analogWrite(io17,int(float(doc["io17"])*255));
  const char* tx_serial=doc["tx"];
  Serial.println(tx_serial);
  
}


void mqttSend() {
  DynamicJsonDocument doc(1024);
  doc["name"]=eeprom_user;
  doc["battery"]=batt_level;
  doc["photo"]=round(photoresistor_value/4096.0*100)/100.0;//normalize
  doc["pot"]=round(potentiometer_value/4096.0*100)/100.0;
  // param1["magnet"]=;
  doc["button"]=button_value;
  doc["rx"]=rx_msg;
  doc["io17"]=io17_value;
  doc["mic"]=mic_event;
  JsonObject acc = doc.createNestedObject("acc");
  
  if(acc_success){
    acc["x"]=acc_x;
    acc["y"]=acc_y;
    acc["z"]=acc_z;
    acc["heading"]=heading;
    // Serial.println(acc_x);
  }
  
 
  

  char output[1024];
  size_t n = serializeJson(doc, output);
  // String output;
  serializeJson(doc, output,n);
  // Serial.println("sending mqtt");
  char userName[40] = "";
  eeprom_user.toCharArray(userName, sizeof(userName));
  client.publish(userName, output);
  // Serial.println(output);
}
