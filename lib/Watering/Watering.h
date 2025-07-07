#ifndef __Watering_H
#define __Watering_H

#include <Arduino.h>
#include <EEPROM.h>

struct WaterLevel
{
    long distance;
    long waterLevel;
    long litersRemaining;
};

class Watering
{
public:
  static bool pumpIsRunning;
  static void initialize();
  static bool checkWaterLevel();
  static WaterLevel getWaterLevel();
  static void turnOnPump1();
  static void turnOnPump2();
  static void turnOffPump1();
  static void turnOffPump2();

private:
  static void turnOnPump(int, int);
  static void turnOffPump(int, int);
};

#endif