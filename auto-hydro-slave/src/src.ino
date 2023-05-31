#include "Arduino.h"
#include "input-module.h"
#include "output-module.h"
#include "ph-sens.h"
#include "sensor-module.h"
#include "serial-com.h"
#include "sonar.h"
#include "tds-sens.h"

void sensorRoutine();

SensorModule sensor;
float value[6];

DigitalOut ledGreen(A6);
DigitalOut ledRed(A7);
DigitalOut buzzer(12);

DigitalOut pumpNutritionA(A5, true);
DigitalOut pumpNutritionB(A4, true);
DigitalOut pumpBasePH(A3, true);
DigitalOut pumpCirculation(A2, true);

SerialCom com;

void setup() {
        Serial.begin(9600);
        sensor.addModule(new Sonar(4, 5));
        sensor.addModule(new Sonar(6, 7));
        sensor.addModule(new Sonar(8, 9));
        sensor.addModule(new Sonar(10, 11));
        sensor.addModule(new PHsens(A0));
        sensor.addModule(new TDS(A1));
        sensor.init();
}

void loop() {
        sensor.update(sensorRoutine);
        com.clearData();
        for (uint8_t i = 0; i < 6; i++) {
                com.addData(value[i]);
        }
        com.sendData();
}

void sensorRoutine() {
        for (uint8_t i = 0; i < 6; i++) {
                sensor.getModule(i)->getSensorValue(&value[i]);
        }
}