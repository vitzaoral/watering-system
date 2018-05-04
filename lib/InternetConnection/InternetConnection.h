#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266WiFi.h>

class InternetConnection
{
  public:
    bool initializeThingSpeak(void);
    bool initializeBlynk(void);
    void runBlynk(void);
    bool sendWaterLevelToBlynk(long);
};

#endif