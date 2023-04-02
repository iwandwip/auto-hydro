#include "hydro_sens.h"

sens_t sensors;

void setup() {
        Serial.begin(9600);
        hydro_sens_init(&sensors);
}

void loop() {
        hydro_sens_loop(&sensors);
        hydro_sens_debug(&sensors);
}
