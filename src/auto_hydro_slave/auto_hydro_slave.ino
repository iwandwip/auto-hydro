#include "hydro_node.h"

node_t node;

void setup() {
        Serial.begin(9600);
        hydro_node_init(&node);
}

void loop() {
        hydro_node_loop(&node);
}
