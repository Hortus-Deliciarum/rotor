#include "HortusWifi.h"
#include "ArduinoOSCWiFi.h"

typedef struct {
  long current_time;
  long past_time;
  long dur;
  const char * sentence;
  int _state;
} singleton;


const int recv_port = 54321;
singleton f1 = { millis(), 0, 1000, "ciao 1", 0 };
singleton f2 = { millis(), 0, 1000, "ciao 2", 0 };


void setup() {
  Serial.begin(115200);

  HortusWifi("FASTWEB-2yurFq", "yBqCDXC6w8", "192.168.1.100");

  OscWiFi.subscribe(recv_port, "/rotor1/state",
    [](const OscMessage& m) {
      f1._state = int(m.arg<float>(0));
      Serial.println("received");
    });

  OscWiFi.subscribe(recv_port, "/rotor2/state",
    [](const OscMessage& m) {
      f2._state = int(m.arg<float>(0));
      Serial.println("received");
    });
}

void loop() {
  OscWiFi.update();
  check_task(&f1);
  check_task(&f2); 
}

void check_task(singleton *t) {
  t->current_time = millis();
  
  if ((t->current_time - t->past_time) >= t->dur) {
    //Serial.println(t->sentence);
    String sent = String(t->sentence);
    
    if (t->_state == 0)
      sent.toLowerCase();
    else
      sent.toUpperCase();
    Serial.println(sent);
    t->past_time = t->current_time;
  }  
}
