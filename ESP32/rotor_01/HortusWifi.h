#ifndef HortusWifi_h
#define HortusWifi_h

#include "Arduino.h"
#include <WiFi.h>

class HortusWifi
{
  public:
    HortusWifi(const char* ip);
    void conn();
  private:
    String _ip;
}

#endif
