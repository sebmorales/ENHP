//  EXPERIMENTS IN NETWORKED HARDWARE PERFORMANCE
//  SFPC 2022
//  MORAKANA
//
////////

//board name
String eeprom_user = "tomato";


#include<EEPROM.h>
#include <WiFi.h>
#include <DNSServer.h>

#include <MQTT.h>
#include <ESP32Servo.h>
WiFiClient net;
MQTTClient client;

//CAPTIVE PORTAL
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
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
#define buzzer 21
#define out 22

boolean out_value = false;
bool ledStatus = false;

//INPUTS
#define tilt 34
#define button 33  // because it needs to be an RTC_GPIO we need to use 33 instead of 17
#define potentiometer 35
#define photoresistor 32//18
#define performance  16

boolean tilt_value = false;
boolean old_tilt_value = false;
int potentiometer_value = 0;
int old_potentiometer_value = 0;
int photoresistor_value = 0;
int old_photoresistor_value = 0;

//time variables
long int timeAwake = 30000; //in battery saving mode, how long (sec) should the board be awake?
long int timeSinceWake = 0;
long int mqttWait = 400; //to avioid sending to many messages a second and saturating/loosing packs
long int lastMessageSent = 0;
long int lastBlink = 0;
int fastBlink = 200;



void setup() {
  Serial.begin(115200);

  pinMode(out, OUTPUT);
  digitalWrite(out, LOW);   //turn out LED off.
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(button, INPUT);
  pinMode(performance, INPUT);
  pinMode(photoresistor, INPUT);
  pinMode(tilt, INPUT);
  beep(); //beep every time the board turns on

  loadEEPROM(); //load network credentials stored in memory
  initSleep(); // setup variables to later be able to sleep & wakeup

  //Attempt to connect to wifi, if it doesn't work make your own
  //network for people to connect to share wifi credentials.
  if (!attemptToConnect()) {
//    servo.setPeriodHertz(50);
//    servo.attach(servoPin);
//    //MQTT- connect to server
//    client.begin("hardwaremovement.com", 1883, net);
//    client.onMessage(messageReceived);
    initImage();//load sfpc image
    runAsServer(); // create captive portal
    timeSinceWake = millis() + 60000; //give some extra time for person to be able to add their credentials
  }
}

void loop() {
  long int loopTime = millis();
  if (connectedSuccess) {
    client.loop();
    delay(10);  // <- fixes some issues with WiFi stability
    if (!client.connected()) {
      connect();
    }
  } else {
    portal();
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

  
  if (loopTime - lastMessageSent > mqttWait) {
    lastMessageSent = loopTime;
    checkInputs(false);//don't force the input broadcast
  }

  if (analogRead(button) < 3000) { //basically pulled down to ground
    checkInputs(true);
    delay(100);
    digitalWrite(out, HIGH);
  }

  //after we did everything we have to do, check if we need to sleep (save battery)
  //or keep going.
  if (loopTime - timeSinceWake > timeAwake ) {
    checkPerformanceMode();
  }
}

void checkInputs(bool force) {
  boolean change = false;

  tilt_value = analogRead(tilt);
  potentiometer_value = analogRead(potentiometer);
  photoresistor_value = analogRead(photoresistor);

  if (tilt_value != old_tilt_value) {
    change = true;
    old_tilt_value = tilt_value;
  }

  if (abs(old_potentiometer_value - potentiometer_value) > 100) {// give some margin for noise
    old_potentiometer_value = potentiometer_value;
    change = true;
  }


  if (change || force) {
    if (connectedSuccess) {
      mqttSend();
    }
  }
}


void beep() {
  analogWrite(buzzer, 50);
  delay(20);
  analogWrite(buzzer, 0);
}
