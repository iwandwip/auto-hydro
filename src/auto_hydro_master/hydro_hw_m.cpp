/*
 *  hydro_hw.cpp
 *
 *  hydro hardware master c
 *  Created on: 2023. 4. 3
 */

#include "Arduino.h"
#include "hydro_def_m.h"
#include "hydro_hw_m.h"

// internal function
String parseStr(String data, char separator[], int index);

void hydro_hw_init(void) {
        // pass
}

void hydro_hw_loop(data_t* packet) {
        if (Serial.available()) {

                char rxBuffer[250];
                uint8_t rxBufferPtr = 0;

                rxBuffer[rxBufferPtr++] = Serial.read();
                // if (rxBuffer[0] != ':') return;

                while (1) {
                        if (Serial.available()) {
                                rxBuffer[rxBufferPtr++] = Serial.read();
                                if (rxBuffer[rxBufferPtr - 1] == '\n') break;
                        }
                }

                rxBuffer[rxBufferPtr] = 0;
                String data = rxBuffer;

                for (uint8_t i = 0; i < ULTRA_SENS_NUM; i++) packet->ult_data[i] = parseStr(data, ",", i).toInt();
                for (uint8_t i = 4; i < ETC_SENS_NUM + 4; i++) packet->etc_data[i - 4] = parseStr(data, ",", i).toFloat();
                for (uint8_t i = 6; i < RELAY_NUMS + 6; i++) packet->state[i - 6] = parseStr(data, ",", i).toInt();

                Serial.print("[");
                for (uint8_t i = 0; i < ULTRA_SENS_NUM; i++) {
                        Serial.print("ult");
                        Serial.print(i + 1);
                        Serial.print(": ");
                        Serial.print(packet->ult_data[i]);
                        Serial.print("][");
                }
                Serial.print("ph: ");
                Serial.print(packet->etc_data[PH_SENS_ACT]);
                Serial.print("][");
                Serial.print("tds: ");
                Serial.print(packet->etc_data[TDS_SENS_ACT]);
                Serial.print("][");
                for (uint8_t i = 0; i < RELAY_NUMS; i++) {
                        Serial.print("state");
                        Serial.print(i + 1);
                        Serial.print(": ");
                        Serial.print(packet->state[i]);
                        Serial.print("][");
                }
                Serial.print("]");
                Serial.println();
        }
}

String parseStr(String data, char separator[], int index) {
        int found = 0;
        int strIndex[] = { 0, -1 };
        int maxIndex = data.length() - 1;
        for (int i = 0; i <= maxIndex && found <= index; i++) {
                if (data.charAt(i) == separator[0] || i == maxIndex) {
                        found++;
                        strIndex[0] = strIndex[1] + 1;
                        strIndex[1] = (i == maxIndex) ? i + 1 : i;
                }
        }
        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}