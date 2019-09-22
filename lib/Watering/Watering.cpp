#include "Watering.h"

// PINS
#define PUMP_1 D8 // balcony
#define PUMP_2 D7 // bedroom
#define ECHO_TRIG D4
#define ECHO_ECHO D3

// address 1 - power of pump 1
// address 2 - power of pump 2
// warning: defined both in InternetConnection.cpp and Watering.cpp !
#define EEPROM_PUMP1_POWER_ADDRESS 1
#define EEPROM_PUMP2_POWER_ADDRESS 2

bool Watering::pumpIsRunning = false;

void Watering::initialize()
{
    // set pinmodes
    pinMode(PUMP_1, OUTPUT);
    pinMode(PUMP_2, OUTPUT);
    pinMode(ECHO_TRIG, OUTPUT);
    pinMode(ECHO_ECHO, INPUT);
}

WaterLevel Watering::getWaterLevel()
{
    WaterLevel waterLevel = {0, 0};
    long distance = 0;
    for (int i = 0; i < 10; i++)
    {
        digitalWrite(ECHO_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(ECHO_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(ECHO_TRIG, LOW);
        long duration = pulseIn(ECHO_ECHO, HIGH);
        //Calculate the distance (in cm) based on the speed of sound.
        distance = duration / 58.2;
        Serial.print(distance);
        Serial.println(" cm");
        //Delay 100ms before next reading.
        delay(100);
    }

    // maximum and minimum water level in cm
    long min = 40;

    waterLevel.distance = distance;
    waterLevel.waterLevel = min - distance;

    // get usable water column
    Serial.print("Usable water column: ");
    Serial.print(waterLevel.waterLevel);
    Serial.println(" cm");
    return waterLevel;
}

///
bool Watering::checkWaterLevel()
{
    if (Watering::pumpIsRunning)
    {
        WaterLevel waterLevel = Watering::getWaterLevel();
        if (waterLevel.waterLevel <= 0) {
            // turn off both pumps
            digitalWrite(PUMP_1, LOW);
            digitalWrite(PUMP_2, LOW);
            Watering::pumpIsRunning = false;
            return true;
        }
    }
    return false;
}

void Watering::turnOnPump1()
{
    Serial.println("Turn ON pump1");
    int pump1Max = EEPROM.read(EEPROM_PUMP1_POWER_ADDRESS);
    Watering::turnOnPump(PUMP_1, pump1Max);
}

void Watering::turnOffPump1()
{
    Serial.println("Turn OFF pump1");
    int pump1Max = EEPROM.read(EEPROM_PUMP1_POWER_ADDRESS);
    Watering::turnOffPump(PUMP_1, pump1Max);
}

void Watering::turnOnPump2()
{
    Serial.println("Turn ON pump2");
    int pump2Max = EEPROM.read(EEPROM_PUMP2_POWER_ADDRESS);
    Watering::turnOnPump(PUMP_2, pump2Max);
}

void Watering::turnOffPump2()
{
    Serial.println("Turn OFF pump2");
    int pump2Max = EEPROM.read(EEPROM_PUMP2_POWER_ADDRESS);
    Watering::turnOffPump(PUMP_2, pump2Max);
}

void Watering::turnOnPump(int pin, int maxValue)
{
    Watering::pumpIsRunning = true;
    for (int i = 0; i <= maxValue; ++i)
    {
        analogWrite(pin, i);
        delay(5);
    }
    // TODO: nejede pak na plny vykon, natvrdo na 100% pokud chci

    digitalWrite(pin, HIGH);
}

void Watering::turnOffPump(int pin, int maxValue)
{
    Watering::pumpIsRunning = false;
    for (int i = maxValue; i >= 0; --i)
    {
        analogWrite(pin, i);
        delay(5);
    }
    digitalWrite(pin, LOW);
}