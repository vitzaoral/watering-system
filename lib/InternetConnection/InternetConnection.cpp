#include "InternetConnection.h"

#include "../../src/settings.cpp"
#include <BlynkSimpleEsp8266.h>

// Blynk virtual pins:
// V1 - water level (long)
// V2 - pump 1
// V3 - pump 2

// MoistureStatus balcoony
// V4, V5, V6, V7, V8

// MoistureStatus bedroom
// V9, V10, V11, V12, V13, V14, V15

// Meteo Data
// V16 - temperature
// V17 - humidity
// V18 - presure
// V19 - meteo data status

WiFiClient client;
Settings settings;

const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// number of attempts to connecting WIFI, API etc.
const int timeout = 10;

// Enable/disable pump1
BLYNK_WRITE(V2)
{
    param.asInt() ? Watering::turnOnPump1() :Watering::turnOffPump1();
}

// Enable/disable pump1
BLYNK_WRITE(V3)
{
    param.asInt() ? Watering::turnOnPump2() :Watering::turnOffPump2();
}

// Initialize WiFi connection and ThingSpeak. Return true if connection is sucessfull.
bool InternetConnection::initializeThingSpeak(void)
{
    WiFi.begin(ssid, password);

    Serial.print("WiFi connecting to: ");
    Serial.println(ssid);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (i == timeout)
        {
            Serial.println("Timeout on WiFi connection");
            return false;
        }
        i++;
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //ThingSpeak.begin(client);
    return true;
}

// Initialize WiFi connection and Blynk. Return true if connection is sucessfull.
bool InternetConnection::initializeBlynk(void)
{
    Serial.println("WiFi connecting to Blynk");
    Blynk.config(blynkAuth);

    // timeout 3sec
    Blynk.connect(1000);

    if (Blynk.connected())
    {
        Blynk.run();
    }

    Serial.println(Blynk.connected() ? "Blynk connected" : "Timeout on or internet connection");
    return Blynk.connected();
}

void InternetConnection::runBlynk(void)
{
    Blynk.run();
}

bool InternetConnection::sendWaterLevelToBlynk(long waterLevel)
{
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V1, waterLevel);
        Serial.println("Send water level to Blynk OK");
        Blynk.run();
        return true;
    }
    else
    {
        Serial.println("Blynk is not connected.");
        return false;
    }
}

bool InternetConnection::sendSoilMoistureToBlynk(SoilMoistureStatus status) {
if (Blynk.connected())
    {
        Blynk.virtualWrite(V4, status.A);
        Blynk.virtualWrite(V5, status.B);
        Blynk.virtualWrite(V6, status.C);
        Blynk.virtualWrite(V7, status.D);
        Blynk.virtualWrite(V8, status.E);
        Serial.println("Send moisture status to Blynk OK");
        Blynk.run();
        return true;
    }
    else
    {
        Serial.println("Blynk is not connected.");
        return false;
    }
}

bool InternetConnection::sendMeteoDataToBlynk(MeteoData meteoData, bool validData)
{
    // create data to send to Blynk
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V16, meteoData.temperature);
        Blynk.virtualWrite(V17, meteoData.humidity);
        Blynk.virtualWrite(V18, meteoData.presure);
        setMeteoDataStatusToBlynk(validData);
        Blynk.run();
        Serial.println("Send data to Blynk OK");
        return true;
    }
    else
    {
        Serial.println("Blynk is not connected");
        return false;
    }
}

// Static method - send message status to Blynk
void InternetConnection::setMeteoDataStatusToBlynk(bool validData)
{
    char *status = (char *)"Data are OK";
    char *color = (char *)"#00FF00";

    if (!validData)
    {
        status = (char *)"Data are invalid";
        color = (char *)"#FF0000";
    }

    Blynk.virtualWrite(V19, status);
    Blynk.setProperty(V19, "color", color);
}