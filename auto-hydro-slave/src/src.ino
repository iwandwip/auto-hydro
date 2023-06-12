#include "Arduino.h"
#include "input-module.h"
#include "output-module.h"
#include "ph-sens.h"
#include "sensor-module.h"
#include "serial-com.h"
#include "sonar.h"
#include "tds-sens.h"

void sensorRoutine();
void onReceive(String data);

SensorModule sensor;
float value[6];
float phValue, waterValue, tdsValue;

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
        com.begin(9600);
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

        Serial.print("| sonar[0]: ");
        Serial.print(value[0]);
        Serial.print("| sonar[1]: ");
        Serial.print(value[1]);
        Serial.print("| sonar[2]: ");
        Serial.print(value[2]);
        Serial.print("| sonar[3]: ");
        Serial.print(value[3]);
        Serial.print("| pH: ");
        Serial.print(value[4]);
        Serial.print("| tds: ");
        Serial.print(value[5]);
        Serial.println();

        com.clearData();
        for (uint8_t i = 0; i < 6; i++) {
                com.addData(value[i]);
        }
        com.sendData(500);
        com.receive(onReceive);

        // static uint32_t toggleTime;
        // if (millis() - toggleTime >= 1000) {
        //         pumpNutritionA.toggle();
        //         pumpNutritionB.toggle();
        //         pumpBasePH.toggle();
        //         pumpCirculation.toggle();
        //         ledGreen.toggle();
        //         ledRed.toggle();
        //         toggleTime = millis();
        // }
}

void sensorRoutine() {
        for (uint8_t i = 0; i < 6; i++) {
                sensor.getModule(i)->getSensorValue(&value[i]);
        }
}

void onReceive(String data) {
        waterValue = com.getData(data, 0);
        phValue = com.getData(data, 1);
        tdsValue = com.getData(data, 2);

        // Serial.print("| waterValue: ");
        // Serial.print(waterValue);
        // Serial.print("| phValue: ");
        // Serial.print(phValue);
        // Serial.print("| tdsValue: ");
        // Serial.print(tdsValue);
        // Serial.print("| data: ");
        // Serial.print(data);
        // Serial.println();
}