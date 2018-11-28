#include <Arduino.h>
#include <MeteoData.h>
#include <InternetConnection.h>
#include <Watering.h>
#include <SoilMoisture.h>
#include <Ticker.h>
#include <EEPROM.h>

const int sendDataToInternetInterval = 30000;

InternetConnection connection;
MeteoData meteoData;

void sendDataToInternet();
Ticker timerSendDataToInternet(sendDataToInternet, sendDataToInternetInterval);

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initialize())
    {
        apisAreConnected = connection.initializeBlynk();
        connection.initializeOTA();
    }
}

void sendDataToInternet()
{
    if (apisAreConnected)
    {
        bool successBlynk = false;
        // WaterLevel waterLevel = Watering::getWaterLevel();
        // TODO: when sensors are hidden (in winter season), comment code to avoid timeouts
        // SoilMoistureStatus soilMoistureStatus = SoilMoisture::getSoilMoistureStatus();
        meteoData.setData();
        successBlynk = connection.sendMeteoDataToBlynk(meteoData, meteoData.dataAreValid());
        // successBlynk &= connection.sendWaterLevelToBlynk(waterLevel);
        // successBlynk &= connection.sendSoilMoistureToBlynk(soilMoistureStatus);

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
    // Two bytes for power of pump1 and pump2
    EEPROM.begin(2);
    Serial.begin(9600);
    delay(100);
    Watering::initialize();
    SoilMoisture::initialize();

    initializeInternetConnection();
    startTimers();
}

void loop()
{
    updateTimers();
    connection.runBlynk();
    connection.handleOTA();
}