
//  EXPERIMENTS IN NETWORKED HARDWARE PERFORMANCE
//
//  V2.02
//
//  SFPC 2023
//  MORAKANA
//
// key changes:
// [next time] add battery reading
// [x] replaced tilt w/ magnetometer
// [x] added   temperature reading
// [x] added relay
// [x] added serial com
// [x] added I2C port
////////

//board name
String eeprom_user = "youboardID23";


#include<EEPROM.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
// #include <MQTT.h>
#include <PubSubClient.h>
#include <Adafruit_MMC56x3.h>

WiFiClient net;
// MQTTClient client;
PubSubClient client(net);

//CAPTIVE PORTAL
const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4);
// IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WiFiServer server(80);
String image;
const char netbase[] = "_ExpHardPerf_";


//network credentials
String eeprom_net, eeprom_pass;
boolean connectedSuccess = false;


//OUTPUTS
Servo servo;
#define servoPin 23
#define out 19
#define relay 32//18

//INPUTS
// #define tilt 34
#define photoresistor 34  
#define potentiometer 35
#define performance  39
#define button 33
#define battery 36

//pins 17 & 18 can be used as Digital IO & Analog output (no analogRead)
#define io17 17
#define io18 18

Adafruit_MMC5603 mmc = Adafruit_MMC5603(12345);


boolean out_value = false;
bool ledStatus = false;

// boolean tilt_value = false;
// boolean old_tilt_value = false;
int potentiometer_value = 0;
int old_potentiometer_value = 0;
int photoresistor_value = 0;
int old_photoresistor_value = 0;
boolean button_value;
int io17_value=0;
int old_io17_value=0;
float batt_level=0;
String rx_msg="hi";
bool new_msg=false;
int mag_x;
int mag_y;
int mag_z;
float temp_c;
boolean mag_success=false;

float old_mag=0;

//time variables
long int timeAwake = 30000; //in battery saving mode, how long (sec) should the board be awake?
long int timeSinceWake = 0;
long int mqttWait = 400; //to avioid sending to many messages a second and saturating/loosing packs
long int lastMessageSent = 0;
long int lastBlink = 0;
int fastBlink = 200;



RTC_DATA_ATTR int bootCount = 0;


void setup() {
  Serial.begin(115200);

  pinMode(out, OUTPUT);
  digitalWrite(out, LOW);   //turn out LED off.
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(io18, OUTPUT);
  
  pinMode(io17, INPUT_PULLUP);
  pinMode(button, INPUT);
  pinMode(performance, INPUT);
  pinMode(photoresistor, INPUT);
  // pinMode(battery, INPUT);
  
  beep(); //beep every time the board turns on

  loadEEPROM(); //load network credentials stored in memory
  initSleep(); // setup variables to later be able to sleep & wakeup

  //Attempt to connect to wifi, if it doesn't work make your own
  //network for people to connect to share wifi credentials.

  //attempt to connect, if had connected before, it's likley you can connect again, so wait longer before making captive portal
  if (!attemptToConnect() && bootCount < 2) {
    initImage();//load sfpc image
    Serial.println("captive portal");
    runAsServer(); // create captive portal
    timeSinceWake = millis() + 60000; //give some extra time for person to be able to add their credentials
  } else {
    int additionalTry = 0;
    //if you had successfully connected before, try a couple more times
    while (!connectedSuccess && additionalTry < 4) {
      Serial.println(additionalTry);
      additionalTry++;
      attemptToConnect();
    }
    if (!connectedSuccess) {
      initImage();//load sfpc image
      runAsServer(); // create captive portal
      timeSinceWake = millis() + 60000; //give some extra time for person to be able to add their credentials
    }
  }

}

void loop() {
  long int loopTime = millis();
  if (connectedSuccess) {
    client.loop();
    delay(20);  // <- fixes some issues with WiFi stability
    if (!client.connected()) {
      connect();
    }
  } else {
    portal();
    delay(20);
    if (loopTime - lastBlink > fastBlink) {
      lastBlink = loopTime;
      ledStatus = !ledStatus;
      if (ledStatus) {
        analogWrite(out, 60);
      } else {
        analogWrite(out, 0);
      }
    }
  }


  if (connectedSuccess && loopTime - lastMessageSent > mqttWait) {
    lastMessageSent = loopTime;
    checkInputs(false);//don't force the input broadcast
  }

  if (connectedSuccess && analogRead(button) < 3600) { //basically pulled down to ground
    checkInputs(true);
    button_value=true;
    delay(100);
    // digitalWrite(out, HIGH);
  }else{
    button_value=false;
  }

  //after we did everything we have to do, check if we need to sleep (save battery)
  //or keep going.

  if (loopTime - timeSinceWake > timeAwake ) {
    checkPerformanceMode();
    
  }

}


void checkInputs(bool force) {
  boolean change = false;
  
  if(mag_success){
  sensors_event_t event;
  mmc.getEvent(&event);
    temp_c = round(mmc.readTemperature()*100)/100.0;
    mag_x=round(event.magnetic.x*100)/100.0;
    mag_y=round(event.magnetic.y*100)/100.0;
    mag_z=round(event.magnetic.z*100)/100.0;
  }
  
  photoresistor_value = analogRead(photoresistor);
  batt_level=round(analogRead(battery)*2*3.3*1.1/4095*100)/100;
  potentiometer_value=analogRead(potentiometer);
  io17_value=digitalRead(io17);
  
  if (mag_success && abs(old_mag - abs(mag_x*mag_y*mag_z)) > 4000) {// give some margin for noise
  old_mag = abs(mag_x*mag_y*mag_z);
  change = true;
  }
  
  if (abs(old_potentiometer_value - potentiometer_value) > 60) {// give some margin for noise
    old_potentiometer_value = potentiometer_value;
    change = true;
  }
  if (abs(old_photoresistor_value - photoresistor_value) > 60) {// give some margin for noise
    old_photoresistor_value = photoresistor_value;
    change = true;
  }

  if (old_io17_value!=io17_value) {// give some margin for noise
    old_io17_value = io17_value;
    change = true;
  }

  if(Serial.available() > 0){
    rx_msg = Serial.readStringUntil('\n');
    change=true;
  }



  if (change || force) {
    if (connectedSuccess) {
      mqttSend();
    }
  }
}


void beep() {
  analogWrite(relay, 200);
  delay(20);
  analogWrite(relay, 0);
}