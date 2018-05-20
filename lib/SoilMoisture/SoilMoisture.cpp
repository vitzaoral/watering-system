#include "SoilMoisture.h"

// PINS
#define MUX_A D0
#define MUX_B D5
#define MUX_C D6
#define ANALOG_INPUT A0

void SoilMoisture::initialize()
{
    // set pinmodes
    pinMode(MUX_A, OUTPUT);
    pinMode(MUX_B, OUTPUT);
    pinMode(MUX_C, OUTPUT);
}

void SoilMoisture::changeMux(int c, int b, int a)
{
    digitalWrite(MUX_A, a);
    digitalWrite(MUX_B, b);
    digitalWrite(MUX_C, c);
}

int SoilMoisture::getMoistureValueFromAnalogValue(float value)
{
    // map(hodnota, minimumPůvodníStupnice, maximumPůvodníStupnice, minimumNovéStupnice, maximumNovéStupnice);
    int result = map(value, 1024, 100, 0, 100);
    Serial.print(" Humidity: ");
    Serial.println(result);
    return result;
}

SoilMoistureStatus SoilMoisture::getSoilMoistureStatus()
{
    SoilMoistureStatus status = {0, 0, 0, 0, 0};
    
    changeMux(LOW, LOW, LOW);
    int value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux
    Serial.print(" Analog value X0/A: ");
    Serial.println(value);
    status.A = getMoistureValueFromAnalogValue(value);

    changeMux(LOW, LOW, HIGH);
    value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux
    Serial.print(" Analog value X1/B: ");
    Serial.println(value);
    status.B = getMoistureValueFromAnalogValue(value);

    changeMux(LOW, HIGH, LOW);
    value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux
    Serial.print(" Analog value X2/C: ");
    Serial.println(value);
    status.C = getMoistureValueFromAnalogValue(value);

    changeMux(LOW, HIGH, HIGH);
    value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 3 pin of Mux
    Serial.print(" Analog value X3/D: ");
    Serial.println(value);
    status.D = getMoistureValueFromAnalogValue(value);

    changeMux(HIGH, LOW, LOW);
    value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 4 pin of Mux
    Serial.print(" Analog value X4/E: ");
    Serial.println(value);
    status.E = getMoistureValueFromAnalogValue(value);

    // changeMux(HIGH, LOW, HIGH);
    // value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 5 pin of Mux

    // changeMux(HIGH, HIGH, LOW);
    // value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 6 pin of Mux

    // changeMux(HIGH, HIGH, HIGH);
    // value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 7 pin of Mux

    return status;
}



