#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLnIMPHU5L"
#define BLYNK_TEMPLATE_NAME "Terasa zavlaha"
#define BLYNK_FIRMWARE_VERSION "2.0.7"

#include "InternetConnection.h"
#include <BlynkSimpleEsp8266.h>
#include "../../src/settings.cpp"

// Blynk virtual pins:
// V1 - water level (long)
// V20 - water distance (long)
// V24 - zbyvajici objem (long)
// V2 - pump 1
// V3 - pump 2

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


// Pump1 power
// V25 - slider settings
// V26 - slider value info

// Pump2 power
// V27 - slider settings
// V28 - slider value info

// V29 - terminal balcon

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

// Synchronize settings from Blynk server with device when internet is connected
BLYNK_CONNECTED()
{
    Blynk.syncAll();
}

String overTheAirURL = "";

BLYNK_WRITE(InternalPinOTA)
{
  Serial.println("OTA Started");
  overTheAirURL = param.asString();
  Serial.print("overTheAirURL = ");
  Serial.println(overTheAirURL);

  HTTPClient http;
  http.begin(client, overTheAirURL);

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, overTheAirURL);
  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.println("[update] Update failed.");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("[update] Update no Update.");
    break;
  case HTTP_UPDATE_OK:
    Serial.println("[update] Update ok."); // may not be called since we reboot the ESP
    break;
  }
}

BLYNK_WRITE(V29) {
  String cmd = param.asStr();
  cmd.trim();                                 
  cmd.toLowerCase();                        

  if (cmd == "restart" || cmd == "reset") {
    terminal.println("> Restarting device...");
    terminal.flush();                         
    delay(100);                              
    ESP.restart();                          
  } else {
    terminal.print("> Unknown command: ");
    terminal.println(cmd);
    terminal.flush();
  }
}

// Enable/disable pump1 (horní)
BLYNK_WRITE(V2) {
  int state = param.asInt();

  // časové razítko
  time_t now = time(nullptr);
  struct tm* tm_info = localtime(&now);
  char buf[32];
  strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", tm_info);

  // zpráva
  String msg = String(buf) + " - Pumpa nahoře " + (state ? "ON" : "OFF");

  // na sériovou linku
  Serial.println(msg);
  // do Blynk terminálu
  terminal.println(msg);
  terminal.flush();

  // vlastní akce
  if (state) {
    Watering::turnOnPump1();
  } else {
    Watering::turnOffPump1();
  }
}

// Enable/disable pump2 (dolní)
BLYNK_WRITE(V3) {
  int state = param.asInt();

  // časové razítko
  time_t now = time(nullptr);
  struct tm* tm_info = localtime(&now);
  char buf[32];
  strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", tm_info);

  // zpráva
  String msg = String(buf) + " - Pumpa dole " + (state ? "ON" : "OFF");

  // na sériovou linku
  Serial.println(msg);
  // do Blynk terminálu
  terminal.println(msg);
  terminal.flush();

  // vlastní akce
  if (state) {
    Watering::turnOnPump2();
  } else {
    Watering::turnOffPump2();
  }
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
        Blynk.virtualWrite(V24, waterLevel.litersRemaining);
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

bool InternetConnection::sendMeteoDataToBlynk(MeteoData meteoData, bool validData)
{
    // create data to send to Blynk
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V16, meteoData.sensorOutdoor.temperature);
        Blynk.virtualWrite(V17, meteoData.sensorOutdoor.humidity);
        // Blynk.virtualWrite(V18, meteoData.presure);
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
    // send settings version
    Blynk.virtualWrite(V31, settings.version);

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

void InternetConnection::turnOffPumpButtons()
{
    Blynk.virtualWrite(V2, false);
    Blynk.virtualWrite(V3, false);
}