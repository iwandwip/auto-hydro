/*
 *  tds-sens.h
 *
 *  tds sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef TDS_SENS_H
#define TDS_SENS_H

#include "Arduino.h"
#include "sensor-module.h"
#include "stdint.h"

class TDS : public BaseSens {
       private:
        uint8_t sensorPin;
        float* tdsValue;
        int buff[30], temp[30];
        int buffIndex;
        float averageVolt, temperature;

        uint32_t update_tm;
        uint16_t sample_tm;
        uint32_t debug_tm;
        uint32_t* cal_tm;

        bool isCalibrate;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
        int getMedianNum(int bArray[], int iFilterLen);

       public:
        TDS();
        TDS(uint8_t __pin, bool enableCalibrate = false);
        ~TDS();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;
        void getSensorValue(float* value) override;
        void getSensorValue(int* value) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t __pin);
};

#endif  // TDS_SENS_H