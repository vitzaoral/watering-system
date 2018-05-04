#include <Arduino.h>

#define PUMP_1 D8
#define PUMP_2 D7

#define ECHO_TRIG D4
#define ECHO_ECHO D3

void setup()
{
    Serial.begin(9600);
    pinMode(PUMP_1, OUTPUT);
    pinMode(PUMP_2, OUTPUT);
    pinMode(ECHO_TRIG, OUTPUT);
    pinMode(ECHO_ECHO, INPUT);
}

void testMosfet()
{
    for (int i = 0; i <= 1024; ++i)
    {
        analogWrite(PUMP_1, i);
        analogWrite(PUMP_2, i);
        delay(5);
    }

    delay(5000);

    for (int i = 1024; i >= 0; --i)
    {
        analogWrite(PUMP_1, i);
        analogWrite(PUMP_2, i);
        delay(5);
    }

    delay(5000);
}

void testDistanceMeasure()
{
    digitalWrite(ECHO_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(ECHO_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(ECHO_TRIG, LOW);
    long duration = pulseIn(ECHO_ECHO, HIGH);
    //Calculate the distance (in cm) based on the speed of sound.
    long distance = duration / 58.2;
    Serial.print(distance);
    Serial.println(" cm");
    //Delay 50ms before next reading.
    delay(500);
}

void loop()
{
    testMosfet();
    testDistanceMeasure();
    testDistanceMeasure();
    testDistanceMeasure();
    testDistanceMeasure();
    testDistanceMeasure();
    Serial.println("Loop ");
}