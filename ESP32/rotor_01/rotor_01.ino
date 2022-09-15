#include "ArduinoOSCWiFi.h"

const char* ssid = "FASTWEB-DAB6F7";
const char* password = "1NJYRZE2T4";
const int recv_port = 54321;

int i;
float f;
String s;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  #ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
  #endif

  initWiFi();
  
  Serial.print("WiFi connected, IP = ");
  Serial.println(WiFi.localIP());

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
}
