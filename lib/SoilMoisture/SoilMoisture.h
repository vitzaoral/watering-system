#ifndef __SoilMoisture_H
#define __SoilMoisture_H

#include <Arduino.h>

struct SoilMoistureStatus
{
    int A;
    int analogA;
    int B;
    int analogB;
    int C;
    int analogC;
    int D;
    int analogD;
    int E;
    int analogE;
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