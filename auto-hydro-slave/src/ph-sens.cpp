/*
 *  ph-sens.cpp
 *
 *  ph sensor c
 *  Created on: 2023. 4. 3
 */

#include "ph-sens.h"

#include "Arduino.h"

#define SENSOR_FILTER_KF 8

PHsens::PHsens()
  : sensorPin(A0) {
        isCalibrate = false;
        phValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
        phFilter = new SimpleKalmanFilter(2, 2, 0.01);
}

PHsens::PHsens(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        phValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
        phFilter = new SimpleKalmanFilter(2, 2, 0.01);
}

PHsens::~PHsens() {
}

void PHsens::init() {
        pinMode(sensorPin, INPUT);
}

void PHsens::update() {
        if (millis() - update_tm >= 500) {
                for (int i = 0; i < 10; i++) {
                        buff[i] = analogRead(sensorPin);
                }
                for (int i = 0; i < 9; i++) {
                        for (int j = i + 1; j < 10; j++) {
                                if (buff[i] > buff[j]) {
                                        temp = buff[i];
                                        buff[i] = buff[j];
                                        buff[j] = temp;
                                }
                        }
                }
                average = 0;
                for (int i = 2; i < 8; i++)
                        average += buff[i];
                if (!isCalibrate) {
                        *phValue = (float)average * 5.0 / 1024.0 / 6.0;
                        *phValue = -5.70 * *phValue + 28.0 - 0.7;
                        *phValue = regressPh(*phValue);
                        // *phValue = phFilter->updateEstimate(*phValue);

                        *phValue = *phValue + (*phValue * SENSOR_FILTER_KF);
                        *phValue /= SENSOR_FILTER_KF + 1;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        phValue[SENS_RET_RAW_DATA] = (float)average * 3.3 / 4096.0 / 6;
                        phValue[SENS_RET_RAW_DATA] = *phValue * 3.5;
                        phValue[SENS_RET_ACT_DATA] = phValue[SENS_RET_RAW_DATA] + (phValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        phValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        phValue[SENS_RET_AVG_DATA] = getSensorAverage(phValue[SENS_RET_ACT_DATA]);
                        // phValue[SENS_RET_FILTERED_DATA] = lowPassFilter(phValue[SENS_RET_ACT_DATA], phValue[SENS_RET_FILTERED_DATA], 0.1);
                        phValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(phValue[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void PHsens::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| phValueRaw: ");
                Serial.print(*phValue);
                Serial.println();
                debug_tm = millis();
        }
}

void PHsens::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| phValueRaw: ");
                Serial.print(phValue[SENS_RET_RAW_DATA]);
                Serial.print("| phValueAct: ");
                Serial.print(phValue[SENS_RET_ACT_DATA]);
                Serial.print("| phValueAvg: ");
                Serial.print(phValue[SENS_RET_AVG_DATA]);
                Serial.print("| phValueFiltered: ");
                Serial.print(phValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void PHsens::getSensorValue(float* value) {
        *value = *phValue;
}

void PHsens::getSensorValue(int* value) {
}

void PHsens::count() {
}

void PHsens::reset() {
}

float PHsens::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *phValue;
        return phValue[c];
}

void PHsens::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float PHsens::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float PHsens::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}

double PHsens::regressPh(double x) {
        double terms[] = {
                1.1915131914333834e+000,
                6.2490838951010319e-001,
                3.6799000666406603e-003,
                1.1898949593098573e-004
        };
        double t = 1;
        double r = 0;
        for (double c : terms) {
                r += c * t;
                t *= x;
        }
        return r;
}