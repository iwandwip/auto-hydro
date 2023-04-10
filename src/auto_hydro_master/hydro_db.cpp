/*
 *  hydro_db_m.h
 *
 *  hydro database master c
 *  Created on: 2023. 4. 3
 */

#include "hydro_db.h"
#include "WiFi.h"

#include "Firebase_ESP_Client.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// internal function
extern String parseStr(String data, char separator[], int index);

TaskHandle_t task_in_core2;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void server_t::init(void) {
        configTime(GMT_OFFSET_WIB, DAYLIGHT_OFFSET, NTP_SERVER);

        xTaskCreatePinnedToCore(
          loop_in_core2,
          "db_task",
          20000,
          NULL,
          1,
          &task_in_core2,
          1);
}

status server_t::login(void) {
        config.api_key = API_KEY;
        config.database_url = DATABASE_URL;
        auth.user.email = USER_EMAIL;
        auth.user.password = USER_PASSWORD;

        config.token_status_callback = tokenStatusCallback;
        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
        Firebase.setDoubleDigits(2);  // default 5
        Firebase.setFloatDigits(2);
        config.timeout.serverResponse = 10 * 10000;
        return true;
}

status server_t::connectToWiFi(const char *SSID, const char *PWD) {
        WiFi.mode(WIFI_OFF);
        delay(1000);
        WiFi.mode(WIFI_STA);
        WiFi.begin(SSID, PWD);
        Serial.print("Connecting to : ");
        Serial.println(SSID);
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(1000);
        }
        Serial.println(WiFi.localIP());
        return true;
}

void server_t::handler(data_t *packet) {
}

void server_t::waitConnection(uint32_t tmr) {
}