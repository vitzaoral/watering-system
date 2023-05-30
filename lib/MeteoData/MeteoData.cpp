#include "MeteoData.h"

Adafruit_BME280 bme;

// Initialize and get metheorological data
MeteoData::MeteoData()
{
    if (!bme.begin(0x76))
    {
        Serial.println("Could not find a valid BMP100 sensor on oaddress 0x45!");
    }
}

void MeteoData::setData(void)
{
    presure = bme.readPressure() / 100.0;
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();

    Serial.print("Presure: ");
    Serial.println(presure);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
}

bool MeteoData::dataAreValid(void)
{
    return temperature <= 100.0 && temperature >= -100.0 &&
           humidity <= 100.0 && humidity >= 0.0 &&
           presure <= 1200.0 && presure >= 800.0;
}
