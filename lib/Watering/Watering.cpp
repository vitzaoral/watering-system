#include "Watering.h"

// PINS
#define PUMP_1 D8
#define PUMP_2 D7
#define ECHO_TRIG D4
#define ECHO_ECHO D3

void Watering::initialize()
{
    // set pinmodes
    pinMode(PUMP_1, OUTPUT);
    pinMode(PUMP_2, OUTPUT);
    pinMode(ECHO_TRIG, OUTPUT);
    pinMode(ECHO_ECHO, INPUT);
}

long Watering::getWaterLevel()
{
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
    long result = min - distance;
    // get usable water column
    Serial.print("Usable water column: ");
    Serial.print(result);
    Serial.println(" cm");
    return result;
}

void Watering::turnOnPump1()
{
    Serial.println("Turn ON pump1");
    // TODO: refactor
    for (int i = 0; i <= 1024; ++i)
    {
        analogWrite(PUMP_1, i);
        delay(5);
    }
}

void Watering::turnOffPump1()
{
    Serial.println("Turn OFF pump1");
    // TODO: refactor
    for (int i = 1024; i >= 0; --i)
    {
        analogWrite(PUMP_1, i);
        delay(5);
    }
}

void Watering::turnOnPump2()
{
    Serial.println("Turn ON pump2");
    // TODO: refactor
    for (int i = 0; i <= 1024; ++i)
    {
        analogWrite(PUMP_2, i);
        delay(5);
    }
}

void Watering::turnOffPump2()
{
    Serial.println("Turn OFF pump2");
    // TODO: refactor
    for (int i = 1024; i >= 0; --i)
    {
        analogWrite(PUMP_2, i);
        delay(5);
    }
}