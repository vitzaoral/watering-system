#ifndef __SoilMoisture_H
#define __SoilMoisture_H

#include <Arduino.h>

struct SoilMoistureStatus
{
    int A;
    int B;
    int C;
    int D;
    int E;
};

class SoilMoisture
{
  public:
    static void initialize();
    static SoilMoistureStatus getSoilMoistureStatus();

  private:
    static void changeMux(int, int, int);
    static int getMoistureValueFromAnalogValue(float);
};

#endif