String header;

void runAsServer() {
  delay(100);

  //there is a problem with phones connecting to the same network more than once,
  //the captive portal won't show up, so we need to change the name all the time
  char captiveNetworkName[50] = "";
  unsigned long ran_net = random(10000);

  
  strcpy_P(captiveNetworkName, netbase);//add the first part of the network name

  strcat(captiveNetworkName,eeprom_user.c_str());

  char* ptr = captiveNetworkName + strlen(captiveNetworkName);
  ultoa(ran_net, ptr, 4);
  Serial.println(captiveNetworkName);
  WiFi.softAP(captiveNetworkName);

  WiFi.mode(WIFI_AP);
  // WiFi.softAP("ESP32-DNSServer");
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  // i  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  server.begin();
  delay(1000);
}




//if no network is detected create a captive portal to allow people to provide their network credentials
void portal() {
  dnsServer.processNextRequest();
  WiFiClient client = server.available();   // listen for incoming clients

  boolean newCredentials = false;
  if (client) {
    // If a new client connects,
    Serial.println("New Client.");
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("/network/") >= 0) {
              Serial.println(header);
              String network = getValue(header, '/', 2);
              String password = getValue(header, '/', 3);
              network.replace("%20"," ");//if the username or password have spaces, make sure these are passed properly
              password.replace("%20"," ");
              //              String user = getValue(header, '/', 4);

              //save data to EEPROM
              //start by cleaning the EEPROM
              for (int i = 0 ; i < eepromSize  ; i++) {
                EEPROM.write(i, 0);
              }
             
              EEPROM.writeString(eepromAddr1, network);
              EEPROM.writeString(eepromAddr2, password);
//              EEPROM.writeString(eepromAddr3, eeprom_user);
              EEPROM.commit();
              newCredentials = true; //try out new credentials
              delay(500);

              //attempting to connect:
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println("</style></head>");
              client.println("<body><h2>Attempting to connect to " + network + "!</h2>");
              client.println("<body><p>You may now close this window (Join other network)!</p>");
              client.println("</body></html>");
              client.println();//end response with blank line

            }
            else {
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println("</style></head>");

              // Web Page Heading
              client.println("<body><h2>Welcome to Experiments in Network Performance!</h2>");
              client.println("<body><h3>If you are seeing this screen, it means that your board needs to be connected to the internet.</h3>");
              client.println(image);

              client.println("<br>");
              client.println("<p>Let's get started by sharing your wifi's network with the board. </p>" );
              client.println("<label for=\"fname\">Network Name:</label><br>");
              client.println("<input type=\"text\" id=\"netName\"><br>");
              client.println("<label for=\"lname\">Password:</label><br>");
              client.println("<input type=\"text\" id=\"netPass\"><br><br>");
              //              client.println("<p>Give this board a unique name, we will use this name later to connect to it on the interent.</p>");
              //              client.println("<label for=\"fname\">Board name:</label><br>");
              //              client.println("<input type=\"text\" id=\"userName\" ><br>");
              client.println("<button onclick=myFunction()>Save</>");

              client.println("<script>");
              client.println("function myFunction(){");
              client.println("var name = document.getElementById(\"netName\").value");
              client.println("var pass = document.getElementById(\"netPass\").value");
              //              client.println("var user = document.getElementById(\"userName\").value");
              //              client.println("var path = \"/network/\"+name+\"/\"+pass+\"/\"+user+\"/\"");
              client.println("var path = \"/network/\"+name+\"/\"+pass+\"/\"");

              client.println("window.location.href=path");
              client.println("}");
              client.println("</script>");

              client.println("</body></html>");

              // The HTTP response ends with another blank line
              client.println();
            }
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  if (newCredentials) {
    attemptToConnect();
//    if (attemptToConnect()) {
//      //connection successful
//    }
  }
}


boolean attemptToConnect() {
  char networkName[40] = "";
  char networkPass[40] = "";
//
//  String net0=eeprom_net;
//  String pass0=eeprom_pass;

//  String net0 = EEPROM.readString(eepromAddr1);
//  String pass0 = EEPROM.readString(eepromAddr2);
//  net0.toCharArray(networkName, sizeof(networkName));
//  pass0.toCharArray(networkPass, sizeof(networkPass));

  eeprom_net.toCharArray(networkName, sizeof(networkName));
  eeprom_pass.toCharArray(networkPass, sizeof(networkPass));


  WiFi.begin(networkName, networkPass);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    counter++;
    if (counter > 15) {
      Serial.print("connection not successful to connect to: ");
      // beep();
      // beep();
      // beep();
      return false;
    }
    Serial.print(".");
    out_value=!out_value;
    digitalWrite(out,out_value);
  }
  Serial.println(F("successfull connection"));
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  connectedSuccess = true;


  servo.setPeriodHertz(50);
  servo.attach(servoPin);
  init_acc();
//  MQTT- connect to server
  //

  client.setServer(mqtt_broker, mqtt_port);
  // client.setServer("hardwaremovement.com", 1883);
  client.setCallback(messageReceived);



  connectedBlink();
  timeSinceWake=millis();
  return true;
}

void connectedBlink() {
  for (int i = 0; i < 20; i++) {
    analogWrite(out, 0);    // turn the LED off by making the voltage LOW
    delay(80);
    analogWrite(out, 60);    // turn the LED off by making the voltage LOW
    delay(30);
  }
  digitalWrite(out, LOW);    // turn the LED off by making the voltage LOW

}



String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
