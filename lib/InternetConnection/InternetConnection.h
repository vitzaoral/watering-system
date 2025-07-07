#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266WiFi.h>
#include <Watering.h>
#include <MeteoData.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

class InternetConnection
{
public:
  bool initialize(void);
  bool initializeBlynk(void);
  void runBlynk(void);
  bool sendWaterLevelToBlynk(WaterLevel);
  bool sendMeteoDataToBlynk(MeteoData, bool);
  void setMeteoDataStatusToBlynk(bool);
  void turnOffPumpButtons(void);
};

#endif