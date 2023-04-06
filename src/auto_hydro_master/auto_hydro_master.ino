#include "hydro_hw_m.h"

data_t data;

void setup() {
        Serial.begin(115200);
        hydro_hw_init();
}

void loop() {
        hydro_hw_loop(&data);
}
