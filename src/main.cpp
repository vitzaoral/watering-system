#include <Arduino.h>
#include <MeteoData.h>
#include <InternetConnection.h>
#include <Watering.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <time.h>

const long gmtOffset_sec    = 3600;    // standardní posun (UTC+1)
const int  daylightOffset_sec = 3600;  // letní čas navíc
const char* ntpServer       = "pool.ntp.org";

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

        WaterLevel waterLevel = Watering::getWaterLevel();
        meteoData.setData();

        successBlynk = connection.sendMeteoDataToBlynk(meteoData, meteoData.dataAreValid());
        successBlynk &= connection.sendWaterLevelToBlynk(waterLevel);

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
    Serial.begin(115200);
     delay(100);

    // Two bytes for power of pump1 and pump2
    EEPROM.begin(2);
    Serial.begin(115200);
    delay(100);
    Watering::initialize();
    meteoData.initializeSensors();

    initializeInternetConnection();

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("SNTP initialized");

    startTimers();
}

void loop()
{
    updateTimers();
    connection.runBlynk();
}