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
float value[3];

DigitalOut ledGreen(2);
DigitalOut nutritionPumpA(3, true);
DigitalOut nutritionPumpB(4, true);
DigitalOut basePHPump(5, true);
DigitalOut circulationPump(6, true);

SerialCom com;

void setup() {
        Serial.begin(9600);
        sensor.addModule(new Sonar(2, 3));
        sensor.addModule(new PHsens(A0));
        sensor.addModule(new TDS(A1));
        sensor.init();
}

void loop() {
        sensor.update(sensorRoutine);
}

void sensorRoutine() {
        for (uint8_t i = 0; i < 3; i++) {
                sensor.getModule(i)->getSensorValue(&value[i]);
        }
}