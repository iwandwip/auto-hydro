#include "HardwareSerial.h"
#include <stdint.h>
#include "hydro_sens.h"
#include "hydro_def.h"
#include "Arduino.h"

#define MAX_ULT_DISTANCE 200
#define ULT_FILTER_KF 8

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
                packet->ult.sensorSonar[i] = new NewPing(packet->ult.sensorEchoPin[i], packet->ult.sensorTriggPin[i], MAX_ULT_DISTANCE);
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
        if (millis() - sens_tmr[PH_TMR] >= 300) {
                sens_tmr[PH_TMR] = millis();
        }
        if (millis() - sens_tmr[TDS_TMR] >= 300) {
                sens_tmr[TDS_TMR] = millis();
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
                        Serial.print(packet->ult.rawSensorData[i]);
                        Serial.print("][");
                }
                Serial.print("]");
                Serial.println();
                dbg_tmr = millis();
        }
}

int16_t getUltrasonicVal(ultrasonic_index_t c) {
}
float getPhValue(sens_t *packet) {
}
float getTdsValue(sens_t *packet) {
}