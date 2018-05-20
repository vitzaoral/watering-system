#ifndef __MeteoData_H
#define __MeteoData_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

class MeteoData
{
public:
  MeteoData();
  float temperature;
  float humidity;
  float presure;
  float realAltitude;

  void setData(void);
  bool dataAreValid(void);
};

#endif
