/*
 *  hydro_def.h
 *
 *  Hydro define
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef HYDRO_DEF_H
#define HYDRO_DEF_H

#include "Arduino.h"

#define ESP_TX_SER              2
#define ESP_RX_SER              3


#define TRIGG_ULT_PIN_UP        4
#define ECHO_ULT_PIN_UP         5

#define TRIGG_ULT_PIN_DOWN      6
#define ECHO_ULT_PIN_DOWN       7

#define TRIGG_ULT_PIN_LEFT      8
#define ECHO_ULT_PIN_LEFT       9

#define TRIGG_ULT_PIN_RIGHT     10
#define ECHO_ULT_PIN_RIGHT      11


#define BUZZER_PIN              12
#define LED_RED                 A7
#define LED_GREEN               A6


#define RELAT_ACT_UP            A5
#define RELAT_ACT_DOWN          A4
#define RELAT_ACT_LEFT          A3
#define RELAT_ACT_RIGHT         A2


#define PH_PIN                  A0
#define TDS_PIN                 A1

#endif  // HYDRO_DEF_H