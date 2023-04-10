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

#define API_KEY                 "AIzaSyDPNr0V4GZqh5_K7Rxp6n-cHTM6mJYX8Zk"
#define DATABASE_URL            "https://greenhouse-8e545-default-rtdb.firebaseio.com/"

#define USER_EMAIL              "greenhouse@gmail.com"
#define USER_PASSWORD           "greenhouse"

#define NTP_SERVER              "pool.ntp.org"
#define GMT_OFFSET_SEC          3600
#define GMT_OFFSET_WIB          GMT_OFFSET_SEC * 7
#define DAYLIGHT_OFFSET         3600

#endif  // HYDRO_DEF_H