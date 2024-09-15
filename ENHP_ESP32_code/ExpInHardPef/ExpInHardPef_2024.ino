#include "arduino_secrets.h"

//  EXPERIMENTS IN NETWORKED HARDWARE PERFORMANCE
//
//  V2.6
//
//  Elekhlekha 2024
//  MORAKANA
//
// key changes:
// [x] programmable though USB-C
// [x] added secret tab for ssid, pass, and mqtt broker
////////


///////
// USER VARIABLES

//board name
String eeprom_user = "tarared";//carrot24

//network settings (in case you want them pre-loaded)
String net_id=SECRET_ssid;
String net_pass=SECRET_net_pass;

//this is the mqtt broker, you can connect to your own. This is independent from the node-red server.
const char mqtt_broker[]=SECRET_broker_url;
int mqtt_port=1883;

bool debug=false;

////////
// BORD VARIABLES


#include<EEPROM.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
// #include <MQTT.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_LSM303_Accel.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);

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
// #define servoPin 5
#define servoPin 23
#define out 19
#define relay 32//18

//INPUTS
// #define tilt 34
#define photoresistor 34  
#define potentiometer 35
#define performance  16
#define mic  39
#define button 17
#define battery 36

//pins 17 & 18 can be used as Digital IO & Analog output (no analogRead)
#define io17 33
#define io18 18



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
float acc_x;
float acc_y;
float acc_z;
float old_acc=0;

float heading=0;
float old_heading=0;
boolean mag_success=false;
bool acc_success=false;


int mic_readings=200;
int mic_current_reading=0;
int mic_min=1024;
int mic_max=0;
int mic_th= 120;//90
boolean mic_event=false;
boolean old_mic=false;

//time variables
long int timeAwake = 30000; //in battery saving mode, how long (sec) should the board be awake?
long int timeSinceWake = 0;
long int mqttWait = 400; //to avioid sending to many messages a second and saturating/loosing packs
long int lastMessageSent = 0;
long int lastBlink = 0;
int fastBlink = 200;


RTC_DATA_ATTR int bootCount = 0;

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

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
    // delay(20);  // <- fixes some issues with WiFi stability
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

  // if (connectedSuccess && analogRead(button) < 3600) { //basically pulled down to ground
  if (connectedSuccess && digitalRead(button) ) { //pulled down to ground
    checkInputs(true);
    if(debug){
      Serial.println("button");
    }
    button_value=true;
    delay(100);
    // digitalWrite(out, HIGH);
  }else{
    button_value=false;
  }
  
  if(listenMic()){
    if(debug){
      Serial.println("mic");
    }
    checkInputs(true);
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
    mag.getEvent(&event);
    float Pi=3.14159;
    heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;
    if (heading < 0){
      heading = 360 + heading;
    }
    if(abs(old_heading-heading)>5){
      old_heading=heading;
      change=true;
      if(debug){
        Serial.println("heading");
      }   
    }
  }
  
  if(acc_success){
    sensors_event_t event;
    accel.getEvent(&event);
    acc_x=event.acceleration.x;
    acc_y=event.acceleration.y;
    acc_z=event.acceleration.z;
    if(abs(old_acc-abs(acc_x*acc_y*acc_z))>4000){
      old_acc=abs(acc_x*acc_y*acc_z);
      change=true;
      if(debug){
        Serial.println("acc");
      }
    }
  }
  
  photoresistor_value = analogRead(photoresistor);
  // batt_level=round(analogRead(battery)*2*3.3*1.1/4095*100)/100;
  potentiometer_value=analogRead(potentiometer);
  io17_value=digitalRead(io17);

  if (abs(old_potentiometer_value - potentiometer_value) > 60) {// give some margin for noise
    old_potentiometer_value = potentiometer_value;
    change = true;
    if(debug){
      Serial.println("pot");
    }
  }
  if (abs(old_photoresistor_value - photoresistor_value) > 60) {// give some margin for noise
    old_photoresistor_value = photoresistor_value;
    change = true;
    if(debug){
      Serial.println("photo");
    }
  }

  if (old_io17_value!=io17_value) {// give some margin for noise
    old_io17_value = io17_value;
    change = true;
    if(debug){
      Serial.println("io");
    }
  }

  if(Serial.available() > 0){
    rx_msg = Serial.readStringUntil('\n');
    change=true;
    if(debug){
      Serial.println("serial");
    }
  }


  if (change || force) {
    if (connectedSuccess) {
      if(debug){
        Serial.print("change: ");
        Serial.print(change);
        Serial.print(" force: ");
        Serial.println(force);
      }
      mqttSend();
    }
  }
}


void beep() {
  analogWrite(relay, 200);
  delay(20);
  analogWrite(relay, 0);
}