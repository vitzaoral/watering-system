#include <Arduino.h>
#include <InternetConnection.h>
#include <Watering.h>
#include <Ticker.h>

InternetConnection connection;

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initializeThingSpeak())
    {
        apisAreConnected = connection.initializeBlynk();
    }
}

void setup()
{
    Serial.begin(9600);
    Watering::initialize();
    initializeInternetConnection();
}

void loop()
{
    connection.runBlynk();
}