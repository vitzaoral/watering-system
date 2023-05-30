#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLnIMPHU5L"
#define BLYNK_TEMPLATE_NAME "Terasa zavlaha"
#define BLYNK_FIRMWARE_VERSION "2.0.3"

#include "InternetConnection.h"
#include <BlynkSimpleEsp8266.h>
#include "../../src/settings.cpp"

// Blynk virtual pins:
// V1 - water level (long)
// V20 - water distance (long)
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

// WIFI - balcony system
// V21 - IP address
// V22 - WIFI signal strength

// WIFI - bedroom system
// V23 - IP address
// V24 - WIFI signal strength

// Pump1 power
// V25 - slider settings
// V26 - slider value info

// Pump2 power
// V27 - slider settings
// V28 - slider value info

// V29 - terminal balcon
// V30 - terminal bedroom

// V31 - settings version

WiFiClient client;
Settings settings;

// address 1 - power of pump 1
// address 2 - power of pump 2
// warning: defined both in InternetConnection.cpp and Watering.cpp !
#define EEPROM_PUMP1_POWER_ADDRESS 1
#define EEPROM_PUMP2_POWER_ADDRESS 2

const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// number of attempts to connecting WIFI, API etc.
const int timeout = 10;

// Attach Blynk virtual serial terminal
WidgetTerminal terminal(V29);

// set values to EEPROM
void setToEEPROM(int address, int value)
{
    EEPROM.write(address, value);
    EEPROM.commit();
}

// Enable/disable pump1 - balcony
BLYNK_WRITE(V2)
{
    param.asInt() ? Watering::turnOnPump1() : Watering::turnOffPump1();
}

// Enable/disable pump2 - bedroom
BLYNK_WRITE(V3)
{
    param.asInt() ? Watering::turnOnPump2() : Watering::turnOffPump2();
}

// Set pump1 power slider, write back to blynk to confirm show
BLYNK_WRITE(V25)
{
    int requiredPower = param.asInt();
    Blynk.virtualWrite(V26, requiredPower);
    Serial.println("Target power for pump 1 is " + String(requiredPower) + "%%");
    // map(hodnota, minimumPůvodníStupnice, maximumPůvodníStupnice, minimumNovéStupnice, maximumNovéStupnice);
    int result = map(requiredPower, 0, 100, 0, 1024);
    setToEEPROM(EEPROM_PUMP1_POWER_ADDRESS, result);
}

// Set tepump2 power slider, write back to blynk to confirm show
BLYNK_WRITE(V27)
{
    int requiredPower = param.asInt();
    Blynk.virtualWrite(V28, requiredPower);
    Serial.println("Target power for pump 2 is " + String(requiredPower) + "%%");
    // map(hodnota, minimumPůvodníStupnice, maximumPůvodníStupnice, minimumNovéStupnice, maximumNovéStupnice);
    int result = map(requiredPower, 0, 100, 0, 1024);
    setToEEPROM(EEPROM_PUMP2_POWER_ADDRESS, result);
}

// Initialize WiFi connection. Return true if connection is sucessfull.
bool InternetConnection::initialize(void)
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
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wifi signal stregth: ");
    Serial.println(WiFi.RSSI());

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
    bool connected = Blynk.connected();

    if (connected)
    {
        // send local IP address and WIFI signal stregth
        Blynk.virtualWrite(V21, WiFi.localIP().toString());
        Blynk.virtualWrite(V22, WiFi.RSSI());
    }
    return connected;
}

void InternetConnection::runBlynk(void)
{
    Blynk.run();
}

bool InternetConnection::sendWaterLevelToBlynk(WaterLevel waterLevel)
{
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V1, waterLevel.waterLevel);
        Blynk.virtualWrite(V20, waterLevel.distance);
        Serial.println("Send water level to Blynk OK");
        // send local IP address and WIFI signal stregth
        Blynk.virtualWrite(V21, WiFi.localIP().toString());
        Blynk.virtualWrite(V22, WiFi.RSSI());
        
        Blynk.run();
        return true;
    }
    else
    {
        Serial.println("Blynk is not connected.");
        return false;
    }
}

bool InternetConnection::sendSoilMoistureToBlynk(SoilMoistureStatus status)
{
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V4, status.A);
        Blynk.virtualWrite(V5, status.B);
        Blynk.virtualWrite(V6, status.C);
        Blynk.virtualWrite(V7, status.D);
        Blynk.virtualWrite(V8, status.E);
        Serial.println("Send moisture status to Blynk OK");

        String terminalInfo =
            String("A: analog: " + String(status.analogA) + "   humidity: " + String(status.A) + "%\n") +
            String("B: analog: " + String(status.analogB) + "   humidity: " + String(status.B) + "%\n") +
            String("C: analog: " + String(status.analogC) + "   humidity: " + String(status.C) + "%\n") +
            String("D: analog: " + String(status.analogD) + "   humidity: " + String(status.D) + "%\n") +
            String("E: analog: " + String(status.analogE) + "   humidity: " + String(status.E) + "%\n") +
            "\n--------------\n";
        terminal.println(terminalInfo);
        terminal.flush();

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

    // send settings version
    Blynk.virtualWrite(V31, settings.version);
}

// Run OTA in loop
void InternetConnection::handleOTA(void)
{
    ArduinoOTA.handle();
}

void InternetConnection::initializeOTA(void)
{
    ArduinoOTA.setHostname(settings.hostNameOTA);
    ArduinoOTA.setPassword(settings.passwordOTA);
    ArduinoOTA.begin();
}

void InternetConnection::turnOffPumpButtons()
{
    Blynk.virtualWrite(V2, false);
    Blynk.virtualWrite(V3, false);
}