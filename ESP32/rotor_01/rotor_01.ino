#include "ArduinoOSCWiFi.h"
#include "HortusWifi.h"

const char* ssid = "FASTWEB-DAB6F7";
const char* password = "1NJYRZE2T4";
const int recv_port = 54321;
const int send_port = 5000;

int i;
float f;
String s;


void setup() {
  Serial.begin(115200);
  delay(2000);

  HortusWifi(ssid, password, "192.168.1.125");

  OscWiFi.subscribe(recv_port, "/*",
    [](const OscMessage& m) {
      printMessage(m);
    });

  OscWiFi.subscribe(recv_port, "/*/*",
    [](const OscMessage& m) {
      printMessage(m);
    });

  OscWiFi.subscribe(recv_port, "/*/*/*",
    [](const OscMessage& m) {
      printMessage(m);
    });
}

void loop() {
  OscWiFi.update();
}

void printMessage(const OscMessage& m) {

  int _size = (int)m.size();
  
  Serial.print(m.remoteIP());
  Serial.print(" ");
  Serial.print(m.remotePort());
  Serial.print(" ");
  Serial.print(m.size());
  Serial.print(" ");
  Serial.print(m.address());
  //Serial.print(" ");
  //Serial.print(m.arg<float>(0));

  for (int i=0; i<_size; i++) {
    Serial.print(" ");
    Serial.print(m.arg<float>(i));
  }
  
  Serial.println(); 
  OscWiFi.send(m.remoteIP(), m.remotePort(), "/callback", 1); 
}
