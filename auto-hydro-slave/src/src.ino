#include "Arduino.h"
#include "input-module.h"
#include "output-module.h"
#include "ph-sens.h"
#include "sensor-module.h"
#include "serial-com.h"
#include "sonar.h"
#include "tds-sens.h"

// sensor index
#define SONAR_A 0
#define SONAR_B 1
#define SONAR_C 2
#define SONAR_D 3
#define PH_VALUE 4
#define TDS_VALUE 5

// ext value index
#define WATER_THRESHOLD 0
#define PH_THRESHOLD 1
#define TDS_THRESHOLD 2

// constanta
#define PH_THRESHOLD_OFFSET 0.5
#define TDS_THRESHOLD_OFFSET 25

void sensorRoutine();
void onReceive(String data);

SensorModule sensor;
float value[6];
float extValue[3];

DigitalOut ledGreen(A6);
DigitalOut ledRed(A7);
DigitalOut buzzer(12);

DigitalOut pumpNutritionA(A5, true);
DigitalOut pumpNutritionB(A4, true);
DigitalOut pumpUpPH(A3, true);
DigitalOut pumpDownPH(A2, true);

SerialCom com;

void setup() {
        Serial.begin(9600);
        com.begin(115200);
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
        debug();

        com.clearData();
        for (uint8_t i = 0; i < 6; i++) {
                com.addData(value[i]);
        }
        com.sendData(500);
        com.receive(onReceive);

        // if (value[PH_VALUE] > extValue[PH_THRESHOLD] + PH_THRESHOLD_OFFSET) {
        //         pumpUpPH.off();
        //         pumpDownPH.on();
        // } else if (value[PH_VALUE] < extValue[PH_THRESHOLD] - PH_THRESHOLD_OFFSET) {
        //         pumpUpPH.on();
        //         pumpDownPH.off();
        // } else {
        //         pumpUpPH.off();
        //         pumpDownPH.off();
        // }

        // if (value[TDS_VALUE] > extValue[TDS_THRESHOLD] + TDS_THRESHOLD_OFFSET) {
        //         pumpNutritionA.off();
        //         pumpNutritionB.off();
        // } else if (value[TDS_VALUE] < extValue[TDS_THRESHOLD] - TDS_THRESHOLD_OFFSET) {
        //         pumpNutritionA.on();
        //         pumpNutritionB.on();
        // }
}

void sensorRoutine() {
        for (uint8_t i = 0; i < 6; i++) {
                sensor.getModule(i)->getSensorValue(&value[i]);
        }
        // value[PH_VALUE] = 5.75;
        // value[TDS_VALUE] = 1200.0;
}

void onReceive(String data) {
        float buffer[3];
        for (uint8_t i = 0; i < 3; i++) {
                buffer[i] = com.getData(data, i);
                if (buffer[i] != 0) extValue[i] = buffer[i];
        }
}

void debug() {
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

        // Serial.print("| waterThresh: ");
        // Serial.print(extValue[WATER_THRESHOLD]);
        Serial.print("| phThresh: ");
        Serial.print(extValue[PH_THRESHOLD]);
        Serial.print("| tdsThresh: ");
        Serial.print(extValue[TDS_THRESHOLD]);
        Serial.println();
}