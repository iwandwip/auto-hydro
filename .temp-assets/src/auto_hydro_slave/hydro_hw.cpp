/*
 *  hydro_hw.cpp
 *
 *  hydro hardware c
 *  Created on: 2023. 4. 3
 */

#include "Arduino.h"
#include "hydro_hw.h"
#include "hydro_def.h"
#include "SoftwareSerial.h"

#define SEND_TO_ESP
#define SEND_INTERVAL 1000

SoftwareSerial espSerial(ESP_TX_SER, ESP_RX_SER);

void hydro_hw_init(void) {
#ifdef SEND_TO_ESP
        espSerial.begin(115200);
        espSerial.println();
#endif
}

void hydro_hw_loop(sens_t* packet_sens, node_t* packet_node) {
        static uint32_t snd_tmr;
        if (millis() - snd_tmr >= SEND_INTERVAL) {
#ifdef SEND_TO_ESP
                // espSerial.print(":");
                espSerial.print(packet_sens->ult.sensorData[ULTRA_SENS_UP]);
                espSerial.print(",");
                espSerial.print(packet_sens->ult.sensorData[ULTRA_SENS_DOWN]);
                espSerial.print(",");
                espSerial.print(packet_sens->ult.sensorData[ULTRA_SENS_LEFT]);
                espSerial.print(",");
                espSerial.print(packet_sens->ult.sensorData[ULTRA_SENS_RIGHT]);
                espSerial.print(",");
                espSerial.print(packet_sens->ph.phAct);
                espSerial.print(",");
                espSerial.print(packet_sens->tds.tdsValue);
                espSerial.print(",");
                espSerial.print(packet_node->relay.relayState[RELAY_UP]);
                espSerial.print(",");
                espSerial.print(packet_node->relay.relayState[RELAY_DOWN]);
                espSerial.print(",");
                espSerial.print(packet_node->relay.relayState[RELAY_LEFT]);
                espSerial.print(",");
                espSerial.print(packet_node->relay.relayState[RELAY_RIGHT]);
                espSerial.println();
#endif
                snd_tmr = millis();
        }
}