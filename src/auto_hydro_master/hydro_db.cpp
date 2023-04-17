/*
 *  hydro_db_m.h
 *
 *  hydro database master c
 *  Created on: 2023. 4. 3
 */

#include "hydro_db.h"
#include "WiFi.h"
#include "esp32-hal.h"

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
        static uint32_t send_tmr;

        // address
        address ultrasonic_addr[ULTRA_SENS_NUM];
        address relay_addr[RELAY_NUMS];
        address etc_addr[ETC_SENS_NUM];

        ultrasonic_addr[ULTRA_SENS_UP] = ULTRASOINC_0_ADDR;
        ultrasonic_addr[ULTRA_SENS_DOWN] = ULTRASOINC_1_ADDR;
        ultrasonic_addr[ULTRA_SENS_LEFT] = ULTRASOINC_2_ADDR;
        ultrasonic_addr[ULTRA_SENS_RIGHT] = ULTRASOINC_3_ADDR;

        relay_addr[RELAY_UP] = RELAY_0_ADDR;
        relay_addr[RELAY_DOWN] = RELAY_1_ADDR;
        relay_addr[RELAY_LEFT] = RELAY_2_ADDR;
        relay_addr[RELAY_RIGHT] = RELAY_3_ADDR;

        etc_addr[PH_SENS_ACT] = PH_ADDR;
        etc_addr[TDS_SENS_ACT] = TDS_ADDR;

        if (Firebase.ready() && millis() - send_tmr >= 2000) {
                for (int i = 0; i < ULTRA_SENS_NUM; i++)
                        Firebase.RTDB.setIntAsync(&fbdo, (ultrasonic_addr[i]), packet->ult_data[i]) ? 1 : 0;
                for (int i = 0; i < RELAY_NUMS; i++)
                        Firebase.RTDB.setIntAsync(&fbdo, (relay_addr[i]), packet->state[i]) ? 1 : 0;
                for (int i = 0; i < ETC_SENS_NUM; i++)
                        Firebase.RTDB.setFloatAsync(&fbdo, (etc_addr[i]), packet->etc_data[i]) ? 1 : 0;
                send_tmr = millis();
        }
}

void server_t::waitConnection(uint32_t tmr) {
        delay(tmr);
}