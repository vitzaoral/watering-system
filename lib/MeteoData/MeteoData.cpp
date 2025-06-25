#include "MeteoData.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void MeteoData::initializeSensors()
{
    if (!sht31.begin(0x44))
    {
        Serial.println("Could not find a valid SHT31 sensor on address 0x44!");
    }
    else
    {
        Serial.println("Outdoor SHT31 OK");
    }
}

void MeteoData::setData()
{
    // int delayTime = 300;
    Serial.println("");
    Serial.print("Outdoor sensor: ");
    sensorOutdoor.temperature = sht31.readTemperature();
    sensorOutdoor.humidity = sht31.readHumidity();
    MeteoData::printSensorData(&sensorOutdoor);
}

void MeteoData::printSensorData(TempAndHumidity *data)
{
    Serial.print("temperature: " + String(data->temperature) + "°C ");
    Serial.println("humidity: " + String(data->humidity) + "% ");
    Serial.println("");
}

bool MeteoData::dataAreValid()
{
    return sensorOutdoor.temperature <= 100.0 && sensorOutdoor.temperature >= -100.0 &&
           sensorOutdoor.humidity <= 100.0 && sensorOutdoor.humidity >= 0.0;
}