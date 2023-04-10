/*
 *  hydro_def_m.h
 *
 *  Hydro define master header
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef HYDRO_DEF_H
#define HYDRO_DEF_H

#include "Arduino.h"

#define API_KEY                 "AIzaSyBMw1RXAB9c-wyiidmtMNcsoRtNlR4D79o"
#define DATABASE_URL            "https://auto-hydro-default-rtdb.firebaseio.com/"

#define USER_EMAIL              "autohydro@hydro.com"
#define USER_PASSWORD           "autohydro"

#define NTP_SERVER              "pool.ntp.org"
#define GMT_OFFSET_SEC          3600
#define GMT_OFFSET_WIB          GMT_OFFSET_SEC * 7
#define DAYLIGHT_OFFSET         3600

#define ULTRASOINC_0_ADDR       "/sensors/ultrasonic-0"
#define ULTRASOINC_1_ADDR       "/sensors/ultrasonic-1"
#define ULTRASOINC_2_ADDR       "/sensors/ultrasonic-2"
#define ULTRASOINC_3_ADDR       "/sensors/ultrasonic-3"

#define PH_ADDR                 "/sensors/ph"
#define TDS_ADDR                "/sensors/tds"

#define RELAY_0_ADDR            "node/relay-0"
#define RELAY_1_ADDR            "node/relay-1"
#define RELAY_2_ADDR            "node/relay-2"
#define RELAY_3_ADDR            "node/relay-3"

#endif  // HYDRO_DEF_H