#include "Arduino.h"
#include "firebase-handler.h"
#include "input-module.h"
#include "output-module.h"
#include "serial-com.h"

void onReceive(String data);

FirebaseModule firebase;
SerialCom com;

float value[6];

void setup() {
        Serial.begin(115200);
        firebase.connectToWiFi("Polinema Hotspot AJ LT 3 Tengah", "");
        firebase.waitConnection(1000);
        firebase.init();
}

void loop() {
        com.receive(onReceive);
}

void serverHandler(void* pvParameter) {
        firebase.waitConnection(3000);
        for (;;) {
                if (firebase.isConnect()) {
                }
                vTaskDelay(20 / portTICK_PERIOD_MS);
        }
}

void onReceive(String data) {
        for (uint8_t i = 0; i < 6; i++) {
                value[i] = com.getData(data, i);
        }
}