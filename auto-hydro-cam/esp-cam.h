/*
 *  esp-cam.h
 *
 *  esp camera module
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef ESP_CAM_H
#define ESP_CAM_H

#include "Arduino.h"
#include "WiFi.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include "config.h"
#include "SPIFFS.h"
#include "FS.h"
#include "esp-cam-config.h"
#include "firebase-handler.h"

class EspCamera {
      private:
        uint32_t captureTime;
        String imgStorePath;

        bool isSPIFFS;
        bool isCheck;
        bool isDoneCapturing;
        bool isUsingFlash;
      public:
        EspCamera();
        ~EspCamera();
        void initSPIFFS();
        void init(bool _isUsingFlash = true);
        void capture(uint32_t _time = 5000);
        bool check(fs::FS &fs);
        bool doneCapturing();

        void setPath(String _path);
        String getPath();

        void setFrameSize(int _frame_size);
        void setJpegQuality(int _size);
        void setFbCount(int _size);

        void setConfig(camera_config_t _config);
        void setMinPicSize(int _size);
};

#endif  // ESP_CAM_H
