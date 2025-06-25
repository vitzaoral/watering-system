#ifndef __MeteoData_H
#define __MeteoData_H

#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"


struct TempAndHumidity
{
    float temperature;
    float humidity;
};

class MeteoData
{
  public:
    TempAndHumidity sensorOutdoor;
    void initializeSensors();
    void setData();
    bool dataAreValid();

  private:
    void printSensorData(TempAndHumidity *data);
};

#endif
