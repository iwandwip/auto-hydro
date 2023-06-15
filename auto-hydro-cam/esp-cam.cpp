/*
 *  esp-cam.cpp
 *
 *  esp camera c
 *  Created on: 2023. 4. 3
 */

#include "esp-cam.h"

EspCamera::EspCamera()
  : isSPIFFS(false), isCheck(false), isDoneCapturing(false), isUsingFlash(false) {
}

EspCamera::~EspCamera() {
}

void EspCamera::initSPIFFS() {
        if (!SPIFFS.begin(true)) {
                Serial.println("An Error has occurred while mounting SPIFFS");
                ESP.restart();
        } else {
                delay(500);
                Serial.println("SPIFFS mounted successfully");
        }
}

void EspCamera::init(bool _isUsingFlash) {
        isUsingFlash = _isUsingFlash;
        if (isUsingFlash) {
                pinMode(ESPCAM_FLASH_PIN, OUTPUT);
        }
        // OV2640 camera module
        camera_config_t config;
        config.ledc_channel = LEDC_CHANNEL_0;
        config.ledc_timer = LEDC_TIMER_0;
        config.pin_d0 = Y2_GPIO_NUM;
        config.pin_d1 = Y3_GPIO_NUM;
        config.pin_d2 = Y4_GPIO_NUM;
        config.pin_d3 = Y5_GPIO_NUM;
        config.pin_d4 = Y6_GPIO_NUM;
        config.pin_d5 = Y7_GPIO_NUM;
        config.pin_d6 = Y8_GPIO_NUM;
        config.pin_d7 = Y9_GPIO_NUM;
        config.pin_xclk = XCLK_GPIO_NUM;
        config.pin_pclk = PCLK_GPIO_NUM;
        config.pin_vsync = VSYNC_GPIO_NUM;
        config.pin_href = HREF_GPIO_NUM;
        config.pin_sscb_sda = SIOD_GPIO_NUM;
        config.pin_sscb_scl = SIOC_GPIO_NUM;
        config.pin_pwdn = PWDN_GPIO_NUM;
        config.pin_reset = RESET_GPIO_NUM;
        config.xclk_freq_hz = 20000000;
        config.pixel_format = PIXFORMAT_JPEG;

        if (psramFound()) {
                config.frame_size = FRAMESIZE_UXGA;
                config.jpeg_quality = 62;
                config.fb_count = 2;
        } else {
                config.frame_size = FRAMESIZE_SVGA;
                config.jpeg_quality = 62;
                config.fb_count = 1;
        }
        // Camera init
        esp_err_t err = esp_camera_init(&config);
        if (err != ESP_OK) {
                Serial.printf("Camera init failed with error 0x%x", err);
                ESP.restart();
        }
}

void EspCamera::capture(uint32_t _time) {
        if (millis() - captureTime >= _time) {
                captureTime = millis();
                if (isUsingFlash) {
                        digitalWrite(ESPCAM_FLASH_PIN, HIGH);
                }
                isDoneCapturing = false;
                camera_fb_t *fb = NULL;
                bool ok = 0;
                do {
                        Serial.println("ESP32-CAM capturing photo...");
                        fb = esp_camera_fb_get();
                        if (!fb) {
                                Serial.println("Failed");
                                return;
                        }
                        Serial.printf("Picture file name: %s\n", STORAGE_PATH);
                        File file = SPIFFS.open(STORAGE_PATH, FILE_WRITE);
                        if (!file) {
                                Serial.println("Failed to open file in writing mode");
                        } else {
                                file.write(fb->buf, fb->len);
                                Serial.print("The picture has been saved in ");
                                Serial.print(STORAGE_PATH);
                                Serial.print(" - Size: ");
                                Serial.print(file.size());
                                Serial.println(" bytes");
                        }
                        file.close();
                        esp_camera_fb_return(fb);
                        ok = check(SPIFFS);
                } while (!ok);
                if (isUsingFlash) {
                        digitalWrite(ESPCAM_FLASH_PIN, LOW);
                }
                isDoneCapturing = true;
        }
}

bool EspCamera::check(fs::FS &fs) {
        File f_pic = fs.open(STORAGE_PATH);
        unsigned int pic_sz = f_pic.size();
        return (pic_sz > 100);
}

bool EspCamera::doneCapturing() {
        return isDoneCapturing;
}

void EspCamera::setPath(String _path) {
        imgStorePath = _path;
}

String EspCamera::getPath() {
        return imgStorePath;
}

void EspCamera::setFrameSize(int _frame_size) {
}

void EspCamera::setJpegQuality(int _size) {
}

void EspCamera::setFbCount(int _size) {
}


void EspCamera::setConfig(camera_config_t _config) {
}

void EspCamera::setMinPicSize(int _size) {
}
