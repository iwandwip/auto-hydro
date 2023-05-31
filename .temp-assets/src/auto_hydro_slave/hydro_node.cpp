/*
 *  hydro_node.cpp
 *
 *  hydro node c
 *  Created on: 2023. 4. 3
 */

#include "hydro_node.h"
#include "hydro_hw.h"
#include "Arduino.h"

sens_t sensors;

void hydro_node_init(node_t *packet) {
        packet->relay.relayPin[RELAY_UP] = RELAT_ACT_UP;
        packet->relay.relayPin[RELAY_DOWN] = RELAT_ACT_DOWN;
        packet->relay.relayPin[RELAY_LEFT] = RELAT_ACT_LEFT;
        packet->relay.relayPin[RELAY_RIGHT] = RELAT_ACT_RIGHT;

        packet->indc.indcPin[LED_RED_C] = LED_RED;
        packet->indc.indcPin[LED_GREEN_C] = LED_GREEN;
        packet->indc.indcPin[BUZZER_C] = BUZZER_PIN;

        for (uint8_t i = 0; i < RELAY_NUMS; i++) {
                pinMode(packet->relay.relayPin[i], OUTPUT);
                digitalWrite(packet->relay.relayPin[i], HIGH);
        }

        for (uint8_t i = 0; i < INDC_NUMS; i++) {
                pinMode(packet->indc.indcPin[i], OUTPUT);
                digitalWrite(packet->indc.indcPin[i], LOW);
        }

        // digitalWrite(packet->indc.indcPin[LED_RED_C], HIGH);
        // digitalWrite(packet->indc.indcPin[LED_GREEN_C], HIGH);

        hydro_hw_init();
        hydro_sens_init(&sensors);
}

void hydro_node_loop(node_t *packet) {
        hydro_sens_loop(&sensors);
        hydro_sens_debug(&sensors);
        hydro_hw_loop(&sensors, packet);
}

void hydro_node_debug(node_t *packet) {
}