#include "firebase-handler.h"
#include "esp-cam.h"

FirebaseModule firebase;
EspCamera espcam;

void setup() {
        Serial.begin(115200);
        firebase.connectToWiFi("KASTARA INDONESIAA", "KASTARA@2022");
        firebase.waitConnection(1000);
        firebase.init();

        espcam.setPath(STORAGE_PATH);
        espcam.initSPIFFS();
        WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
        espcam.init();
}

void loop() {
        espcam.capture(15000);
}

void serverHandler(void* pvParameter) {
        firebase.waitConnection(3000);
        for (;;) {
                if (firebase.isConnect()) {
                        if (espcam.check(SPIFFS))
                                firebase.sendFilesToStorage(STORAGE_PATH);
                }
                vTaskDelay(20 / portTICK_PERIOD_MS);
        }
}
