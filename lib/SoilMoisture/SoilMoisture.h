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
    void initialize();
    SoilMoistureStatus getSoilMoistureStatus();

  private:
    void changeMux(int, int, int);
    int getMoistureValueFromAnalogValue(float);
};

#endif