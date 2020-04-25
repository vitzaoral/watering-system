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
void checkWaterLevel();
Ticker timerSendDataToInternet(sendDataToInternet, sendDataToInternetInterval);
Ticker timerCheckWaterLevel(checkWaterLevel, 4500);

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

void checkWaterLevel()
{
    
    if (Watering::checkWaterLevel())
    {
        connection.turnOffPumpButtons();
    }
}

void sendDataToInternet()
{
    if (apisAreConnected)
    {
        bool successBlynk = false;

        // TODO: for winter mode -> comment WaterLevel & SoilMoistureStatus
        WaterLevel waterLevel = Watering::getWaterLevel();
        SoilMoistureStatus soilMoistureStatus = SoilMoisture::getSoilMoistureStatus();
        meteoData.setData();

        successBlynk = connection.sendMeteoDataToBlynk(meteoData, meteoData.dataAreValid());
        // TODO: for winter mode -> comment sendWaterLevelToBlynk & sendSoilMoistureToBlynk
        successBlynk &= connection.sendWaterLevelToBlynk(waterLevel);
        successBlynk &= connection.sendSoilMoistureToBlynk(soilMoistureStatus);

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
    timerCheckWaterLevel.start();
}

void updateTimers()
{
    timerSendDataToInternet.update();
    timerCheckWaterLevel.update();
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