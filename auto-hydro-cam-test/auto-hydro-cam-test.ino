#include "WiFi.h"
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <SPIFFS.h>
#include <FS.h>
#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>

const char* ssid = "TP";
const char* password = "00000000";

#define API_KEY "AIzaSyCJgnZh9hEy_4dSLUSuLRlVw4lo6P3XmU8"
#define USER_EMAIL "auto-hydro@gmail.com"
#define USER_PASSWORD "auto-hydro"
#define STORAGE_BUCKET_ID "auto-hydro-barra.appspot.com"
#define FILE_PHOTO "/data/photo.jpg"

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

boolean takeNewPhoto = true;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

bool taskCompleted = false;

bool checkPhoto(fs::FS& fs) {
        File f_pic = fs.open(FILE_PHOTO);
        unsigned int pic_sz = f_pic.size();
        return (pic_sz > 100);
}

void capturePhotoSaveSpiffs(void) {
        camera_fb_t* fb = NULL;
        bool ok = 0;
        do {
                Serial.println("Taking a photo...");
                fb = esp_camera_fb_get();
                if (!fb) {
                        Serial.println("Camera capture failed");
                        return;
                }
                Serial.printf("Picture file name: %s\n", FILE_PHOTO);
                File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
                if (!file) {
                        Serial.println("Failed to open file in writing mode");
                } else {
                        file.write(fb->buf, fb->len);
                        Serial.print("The picture has been saved in ");
                        Serial.print(FILE_PHOTO);
                        Serial.print(" - Size: ");
                        Serial.print(file.size());
                        Serial.println(" bytes");
                }
                file.close();
                esp_camera_fb_return(fb);
                ok = checkPhoto(SPIFFS);
        } while (!ok);
}

void initWiFi() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
                Serial.println("Connecting to WiFi...");
        }
}

void initSPIFFS() {
        if (!SPIFFS.begin(true)) {
                Serial.println("An Error has occurred while mounting SPIFFS");
                ESP.restart();
        } else {
                delay(500);
                Serial.println("SPIFFS mounted successfully");
        }
}

void initCamera() {
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
        esp_err_t err = esp_camera_init(&config);
        if (err != ESP_OK) {
                Serial.printf("Camera init failed with error 0x%x", err);
                ESP.restart();
        }
}

void setup() {
        Serial.begin(115200);
        initWiFi();
        initSPIFFS();
        WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
        initCamera();
        configF.api_key = API_KEY;
        auth.user.email = USER_EMAIL;
        auth.user.password = USER_PASSWORD;
        configF.token_status_callback = tokenStatusCallback;
        Firebase.begin(&configF, &auth);
        Firebase.reconnectWiFi(true);
        pinMode(4, OUTPUT);
}

void loop() {
        if (takeNewPhoto) {
                digitalWrite(4, HIGH);
                capturePhotoSaveSpiffs();
                digitalWrite(4, LOW);
        }
        if (Firebase.ready()) {
                Serial.print("Uploading picture... ");
                if (Firebase.Storage.upload(&fbdo,
                                            STORAGE_BUCKET_ID,
                                            FILE_PHOTO,
                                            mem_storage_type_flash,
                                            FILE_PHOTO,
                                            "image/jpeg")) {
                        Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
                } else {
                        Serial.println(fbdo.errorReason());
                }
        }
}
