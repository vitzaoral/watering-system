#include <Arduino.h>
#include <InternetConnection.h>
#include <Watering.h>
#include <Ticker.h>

const int sendDataToInternetInterval = 60000;

InternetConnection connection;

void sendDataToInternet();
Ticker timerSendDataToInternet(sendDataToInternet, sendDataToInternetInterval);

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initializeThingSpeak())
    {
        apisAreConnected = connection.initializeBlynk();
    }
}

void sendDataToInternet()
{
    if (apisAreConnected)
    {
        long waterLevel = Watering::getWaterLevel();
        bool successBlynk = connection.sendWaterLevelToBlynk(waterLevel);

        if (successBlynk)
        {
            Serial.println("Data was sent");
        }
        else
        {
            Serial.println("No internet connection, try initialize connection");
            apisAreConnected = false;
            initializeInternetConnection();
        }
    }
    else
    {
        initializeInternetConnection();
    }
}

void startTimers()
{
    timerSendDataToInternet.start();
}

void updateTimers()
{
    timerSendDataToInternet.update();
}

void setup()
{
    // TODO: OTA
    Serial.begin(9600);
    delay(100);
    Watering::initialize();
    initializeInternetConnection();
    startTimers();
}

void loop()
{
    updateTimers();
    connection.runBlynk();
}