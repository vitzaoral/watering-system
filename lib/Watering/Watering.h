#ifndef __Watering_H
#define __Watering_H

#include <Arduino.h>

struct WaterLevel
{
    long distance;
    long waterLevel;
};

class Watering
{
public:
  static void initialize();
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