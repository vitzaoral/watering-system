#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266WiFi.h>
#include <Watering.h>
#include <SoilMoisture.h>

class InternetConnection
{
  public:
    bool initializeThingSpeak(void);
    bool initializeBlynk(void);
    void runBlynk(void);
    bool sendWaterLevelToBlynk(long);
    bool sendSoilMoistureToBlynk(SoilMoistureStatus);
};

#endif