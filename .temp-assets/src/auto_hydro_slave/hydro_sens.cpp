/*
 *  hydro_sens.cpp
 *
 *  hydro sensor c
 *  Created on: 2023. 4. 3
 */

#include <stdint.h>
#include "hydro_sens.h"
#include "hydro_def.h"
#include "Arduino.h"
#include "HardwareSerial.h"

#define ULT_MAX_DISTANCE 200
#define ULT_FILTER_KF 8

#define TDS_SENS_BIT 1024.0
#define TDS_TEMPERATURE 25.0
#define TDS_VREF 5

#define PH_SENS_BIT 1024.0
#define PH_TEMPERATURE 27.0
#define PH_VREF 5
#define PH_CAL 21.34 - 12.0

// internal function
int getMedianNum(int bArray[], int iFilterLen);

void hydro_sens_init(sens_t *packet) {
        packet->ult.sensorTriggPin[ULTRA_SENS_UP] = TRIGG_ULT_PIN_UP;
        packet->ult.sensorTriggPin[ULTRA_SENS_DOWN] = TRIGG_ULT_PIN_DOWN;
        packet->ult.sensorTriggPin[ULTRA_SENS_LEFT] = TRIGG_ULT_PIN_LEFT;
        packet->ult.sensorTriggPin[ULTRA_SENS_RIGHT] = TRIGG_ULT_PIN_RIGHT;

        packet->ult.sensorEchoPin[ULTRA_SENS_UP] = ECHO_ULT_PIN_UP;
        packet->ult.sensorEchoPin[ULTRA_SENS_DOWN] = ECHO_ULT_PIN_DOWN;
        packet->ult.sensorEchoPin[ULTRA_SENS_LEFT] = ECHO_ULT_PIN_LEFT;
        packet->ult.sensorEchoPin[ULTRA_SENS_RIGHT] = ECHO_ULT_PIN_RIGHT;

        for (uint8_t i = 0; i < ULTRA_SENS_NUM; i++) {
                packet->ult.sensorSonar[i] = new NewPing(packet->ult.sensorEchoPin[i], packet->ult.sensorTriggPin[i], ULT_MAX_DISTANCE);
                packet->ult.sensorOffset[i] = 0;
        }

        pinMode(PH_PIN, INPUT);
        pinMode(TDS_PIN, INPUT);
}
void hydro_sens_loop(sens_t *packet) {
        static uint32_t sens_tmr[NUM_TMR];
        if (millis() - sens_tmr[ULT_TMR] >= 300) {
                for (uint8_t i = 0; i < ULTRA_SENS_NUM; i++) {
                        // exponential filter
                        packet->ult.rawSensorData[i] = packet->ult.sensorSonar[i]->ping_cm() - packet->ult.sensorOffset[i];
                        packet->ult.sensorData[i] = (packet->ult.rawSensorData[i] + (packet->ult.rawSensorData[i] * ULT_FILTER_KF));
                        packet->ult.sensorData[i] /= ULT_FILTER_KF + 1;
                }
                sens_tmr[ULT_TMR] = millis();
        }
        for (uint8_t i = 0; i < PHSCOUNT; i++) {
                unsigned long t = 30;
                for (static unsigned long e_time; millis() - e_time >= (t); e_time += (t)) {
                        packet->ph.buffer[i] = analogRead(PH_PIN);
                }
        }
        if (millis() - sens_tmr[PH_TMR] >= 300) {
                for (uint8_t i = 0; i < PHSCOUNT; i++) {
                        packet->ph.bufferTemp[i] = packet->ph.buffer[i];
                        packet->ph.phVolt = getMedianNum(packet->ph.bufferTemp, PHSCOUNT) * PH_VREF / PH_SENS_BIT / 6;
                        packet->ph.phAct = -5.70 * packet->ph.phVolt + PH_CAL;
                }
                sens_tmr[PH_TMR] = millis();
        }
        if (millis() - sens_tmr[TDS_TMR] >= 40U) {
                packet->tds.analogBuffer[packet->tds.analogBufferIndex] = analogRead(TDS_PIN);
                packet->tds.analogBufferIndex++;
                if (packet->tds.analogBufferIndex == SCOUNT) packet->tds.analogBufferIndex = 0;
                sens_tmr[TDS_TMR] = millis();
        }
        static uint32_t tdsTmr;  // tdsAvg
        if (millis() - tdsTmr >= 800U) {
                for (uint8_t i = 0; i < SCOUNT; i++) {
                        packet->tds.analogBufferTemp[i] = packet->tds.analogBuffer[i];
                        packet->tds.averageVoltage = getMedianNum(packet->tds.analogBufferTemp, SCOUNT) * (float)TDS_VREF / TDS_SENS_BIT;

                        float cmpCoef = 1.0 + 0.02 * (TDS_TEMPERATURE - 25.0);
                        float cmpVolt = packet->tds.averageVoltage / cmpCoef;

                        packet->tds.tdsValue = (133.42 * cmpVolt * cmpVolt * cmpVolt - 255.86 * cmpVolt * cmpVolt + 857.39 * cmpVolt) * 0.5;
                }
                tdsTmr = millis();
        }
}
void hydro_sens_debug(sens_t *packet) {
        static uint32_t dbg_tmr;
        if (millis() - dbg_tmr >= 30) {
                Serial.print("[");
                for (uint8_t i = 0; i < ULTRA_SENS_NUM; i++) {
                        Serial.print("ult");
                        Serial.print(i + 1);
                        Serial.print(": ");
                        Serial.print(packet->ult.sensorData[i]);
                        Serial.print("][");
                }
                Serial.print("tds: ");
                Serial.print(packet->tds.tdsValue);
                Serial.print("][");
                Serial.print("tdsA: ");
                Serial.print(analogRead(TDS_PIN));
                Serial.print("][");
                Serial.print("ph: ");
                Serial.print(packet->ph.phAct);
                Serial.print("][");
                Serial.print("phA: ");
                Serial.print(analogRead(PH_PIN));
                Serial.print("]");
                Serial.println();
                dbg_tmr = millis();
        }
}

int16_t getUltrasonicVal(sens_t *packet, ultrasonic_index_t c) {
        return packet->ult.sensorData[c];
}
float getPhValue(sens_t *packet) {
        return packet->ph.phAct;
}
float getTdsValue(sens_t *packet) {
        return packet->tds.tdsValue;
}

int getMedianNum(int bArray[], int iFilterLen) {
        int bTab[iFilterLen];
        for (byte i = 0; i < iFilterLen; i++)
                bTab[i] = bArray[i];
        int i, j, bTemp;
        for (j = 0; j < iFilterLen - 1; j++) {
                for (i = 0; i < iFilterLen - j - 1; i++) {
                        if (bTab[i] > bTab[i + 1]) {
                                bTemp = bTab[i];
                                bTab[i] = bTab[i + 1];
                                bTab[i + 1] = bTemp;
                        }
                }
        }
        if ((iFilterLen & 1) > 0) {
                bTemp = bTab[(iFilterLen - 1) / 2];
        } else {
                bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
        }
        return bTemp;
}