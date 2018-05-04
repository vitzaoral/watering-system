#ifndef __Watering_H
#define __Watering_H

#include <Arduino.h>

class Watering
{
  public:
    static void initialize();
    static long getWaterLevel();
    static void turnOnPump1();
    static void turnOnPump2();
    static void turnOffPump1();
    static void turnOffPump2();
};

#endif