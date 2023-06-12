#include "Arduino.h"
#include "firebase-handler.h"
#include "input-module.h"
#include "output-module.h"
#include "serial-com.h"

void onReceive(String data);

FirebaseModule firebase;
SerialCom com;

float value[6];
float dataIn[3];

void setup() {
        Serial.begin(115200);
        firebase.connectToWiFi("KASTARA INDONESIAA", "KASTARA@2022");
        firebase.waitConnection(1000);
        firebase.init();

        dataIn[0] = 0.0;
        dataIn[1] = 0.0;
        dataIn[2] = 0.0;
}

void loop() {
        com.clearData();
        for (uint8_t i = 0; i < 3; i++) {
                com.addData(dataIn[i]);
        }
        com.sendData();
        com.receive(onReceive);
}

void serverHandler(void* pvParameter) {
        firebase.waitConnection(3000);
        for (;;) {
                if (firebase.isConnect()) {
                        firebase.clearData();
                        firebase.addData(value[0], "/sens/sonar-a");
                        firebase.addData(value[1], "/sens/sonar-b");
                        firebase.addData(value[2], "/sens/sonar-c");
                        firebase.addData(value[3], "/sens/sonar-d");
                        firebase.addData(value[4], "/sens/ph-sens");
                        firebase.addData(value[5], "/sens/tds-sens");
                        firebase.sendData(4000);

                        dataIn[0] = firebase.getData("/now-plants/water");
                        dataIn[1] = firebase.getData("/now-plants/ph");
                        dataIn[2] = firebase.getData("/now-plants/tds");

                        Serial.print("Fb In Data ");
                        Serial.print("| ");
                        Serial.print(dataIn[0]);
                        Serial.print("| ");
                        Serial.print(dataIn[1]);
                        Serial.print("| ");
                        Serial.print(dataIn[2]);
                        Serial.println();
                }
                vTaskDelay(20 / portTICK_PERIOD_MS);
        }
}

void onReceive(String data) {
        Serial.print("Serial Data ");
        for (uint8_t i = 0; i < 6; i++) {
                value[i] = com.getData(data, i);
                Serial.print("| ");
                Serial.print(value[i]);
        }
        Serial.println();
}