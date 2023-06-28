/*
 *  sonar.h
 *
 *  hcsr-04 sensor c
 *  Created on: 2023. 4. 3
 */

#include "sonar.h"

#include "SimpleKalmanFilter.h"

#define SONAR_FILTER_KF 8

Sonar::Sonar(bool enableCalibrate) {
        isCalibrate = enableCalibrate;
        distance = (enableCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (enableCalibrate) cal_tm = new uint32_t;
}

Sonar::Sonar(int echo, int trigg, bool enableCalibrate) {
        echoPin = echo;
        triggPin = trigg;
        isCalibrate = enableCalibrate;
        distance = (enableCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (enableCalibrate) cal_tm = new uint32_t;
}

Sonar::~Sonar() {
        delete sonar;
}

void Sonar::init() {
        sonar = new NewPing(triggPin, echoPin, 200);
}

void Sonar::update() {
        if (millis() - update_tm >= 50) {
                if (!isCalibrate) {
                        *distance = sonar->ping_cm();
                        *distance = *distance + (*distance * SONAR_FILTER_KF);
                        *distance /= SONAR_FILTER_KF + 1;
                        *distance = (*distance > 200) ? 200 : *distance;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        distance[SENS_RET_RAW_DATA] = sonar->ping_cm();
                        distance[SENS_RET_ACT_DATA] = distance[SENS_RET_RAW_DATA] + (distance[SENS_RET_RAW_DATA] * SONAR_FILTER_KF);
                        distance[SENS_RET_ACT_DATA] /= SONAR_FILTER_KF + 1;

                        distance[SENS_RET_AVG_DATA] = getSensorAverage(distance[SENS_RET_ACT_DATA]);
                        // distance[SENS_RET_FILTERED_DATA] = lowPassFilter(distance[SENS_RET_ACT_DATA], distance[SENS_RET_FILTERED_DATA], 0.1);
                        distance[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(distance[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }
                update_tm = millis();
        }
}

void Sonar::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| distRaw: ");
                Serial.print(*distance);
                Serial.println();
                debug_tm = millis();
        }
}

void Sonar::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| distRaw: ");
                Serial.print(distance[SENS_RET_RAW_DATA]);
                Serial.print("| distAct: ");
                Serial.print(distance[SENS_RET_ACT_DATA]);
                Serial.print("| distAvg: ");
                Serial.print(distance[SENS_RET_AVG_DATA]);
                Serial.print("| distFiltered: ");
                Serial.print(distance[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void Sonar::getSensorValue(float* value) {
        *value = *distance;
}

void Sonar::getSensorValue(int* value) {
}

void Sonar::count() {
}

void Sonar::reset() {
}

float Sonar::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *distance;
        return distance[c];
}

void Sonar::setPins(uint8_t echo, uint8_t trigg) {
        this->echoPin = echo;
        this->triggPin = trigg;
}

float Sonar::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float Sonar::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}