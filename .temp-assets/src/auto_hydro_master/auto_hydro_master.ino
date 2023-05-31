#include "hydro_hw_m.h"
#include "hydro_db.h"

data_t data;
server_t server;

void setup() {
        Serial.begin(115200);
        hydro_hw_init();

        server.connectToWiFi("KASTARA GROUP INDONESIA",
                             "KASTARA@2022");
        server.login();
        server.init();
}

void loop() {
        hydro_hw_loop(&data);
}

void loop_in_core2(void *pv_param) {
        server.waitConnection(500);
        for (;;) {
                server.handler(&data);
                vTaskDelay(20 / portTICK_PERIOD_MS);
        }
}
