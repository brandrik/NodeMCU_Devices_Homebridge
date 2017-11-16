/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <RCSwitch.h>

RCSwitch receiveSwitch = RCSwitch();
RCSwitch sendSwitch = RCSwitch();


// WIFI SETTINGS
const char* ssid     = "wifi2";
const char* password = "SvEnSkA?20!13.-2016W:";
const bool sendMode = 0; 

void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (sendMode==0){
      Serial.println("Enable Receive"); 
      receiveSwitch.enableReceive(0);
    } else {
      Serial.println("Enable Transmit"); 
      sendSwitch.enableTransmit(5);
    }
    
}

int value = 0;

void loop() {
  if (sendMode == 0) {

    if (receiveSwitch.available()) {
      int value = receiveSwitch.getReceivedValue();
      
      if (value == 0) {
        Serial.print("Unknown encoding");
      } else {
        Serial.print("Received ");
        Serial.print(receiveSwitch.getReceivedValue() );
        Serial.print(" / ");
        Serial.print(receiveSwitch.getReceivedBitlength() );
        Serial.print("bit ");
        Serial.print("Protocol: ");
        Serial.println(receiveSwitch.getReceivedProtocol() );
      }
  
      receiveSwitch.resetAvailable();
    }
  } else {
    // sending mode on
    sendSwitch.send(5526609, 24);
    Serial.print("5526609");
    delay(5000);  
    sendSwitch.send(5526612, 24);
    Serial.print(" / ");
    Serial.print("5526612");
    Serial.print(" / ");
    delay(4000);  
  }

}


