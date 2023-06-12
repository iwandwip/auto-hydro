/*
 *  tds-sens.cpp
 *
 *  tds sensor c
 *  Created on: 2023. 4. 3
 */

#include "tds-sens.h"

#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 8

TDS::TDS()
  : sensorPin(A0) {
        isCalibrate = false;
        tdsValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
        buffIndex = 0;
        temperature = 25;
}

TDS::TDS(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        tdsValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
        buffIndex = 0;
        temperature = 25;
}

TDS::~TDS() {
}

void TDS::init() {
        pinMode(sensorPin, INPUT);
}

void TDS::update() {
        if (millis() - sample_tm >= 40U) {
                buff[buffIndex] = analogRead(sensorPin);
                buffIndex++;
                if (buffIndex == 30) buffIndex = 0;
                sample_tm = millis();
        }
        if (millis() - update_tm >= 800) {
                if (!isCalibrate) {
                        for (int i = 0; i < 30; i++) {
                                temp[i] = buff[i];
                        }
                        averageVolt = getMedianNum(temp, 30) * (float)5.0 / 1024.0;
                        float coeff = 1.0 + 0.02 * (temperature - 25.0);
                        float volt = averageVolt / coeff;
                        *tdsValue = (133.42 * volt * volt * volt - 255.86 * volt * volt + 857.39 * volt) * 0.5;

                        *tdsValue = *tdsValue + (*tdsValue * SENSOR_FILTER_KF);
                        *tdsValue /= SENSOR_FILTER_KF + 1;

                        // *tdsValue = (*tdsValue > 10000) ? 10000 : (*tdsValue < 0) ? 0: *tdsValue;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        tdsValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        tdsValue[SENS_RET_ACT_DATA] = tdsValue[SENS_RET_RAW_DATA] + (tdsValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        tdsValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        tdsValue[SENS_RET_AVG_DATA] = getSensorAverage(tdsValue[SENS_RET_ACT_DATA]);

                        tdsValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(tdsValue[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void TDS::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| tdsValueRaw: ");
                Serial.print(*tdsValue);
                Serial.println();
                debug_tm = millis();
        }
}

void TDS::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| tdsValueRaw: ");
                Serial.print(tdsValue[SENS_RET_RAW_DATA]);
                Serial.print("| tdsValueAct: ");
                Serial.print(tdsValue[SENS_RET_ACT_DATA]);
                Serial.print("| tdsValueAvg: ");
                Serial.print(tdsValue[SENS_RET_AVG_DATA]);
                Serial.print("| tdsValueFiltered: ");
                Serial.print(tdsValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void TDS::getSensorValue(float* value) {
        *value = *tdsValue;
}

void TDS::getSensorValue(int* value) {
}

void TDS::count() {
}

void TDS::reset() {
}

float TDS::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *tdsValue;
        return tdsValue[c];
}

void TDS::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float TDS::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float TDS::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}

int TDS::getMedianNum(int bArray[], int iFilterLen) {
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
        if ((iFilterLen & 1) > 0)
                bTemp = bTab[(iFilterLen - 1) / 2];
        else
                bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
        return bTemp;
}