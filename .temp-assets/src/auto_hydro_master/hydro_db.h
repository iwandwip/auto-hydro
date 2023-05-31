/*
 *  hydro_db_m.h
 *
 *  hydro database master header
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef HYDRO_DB_H
#define HYDRO_DB_H

#include "Arduino.h"
#include "stdint.h"
#include "hydro_hw_m.h"
#include "hydro_def_m.h"

// typedef struct {
//         uint8_t state[4];
// } db_data;

typedef bool status;
typedef String address;

typedef struct {
      private:
        uint32_t conn_tmr;
      public:
        void init(void);
        void handler(data_t* packet);
        void waitConnection(uint32_t tmr);

        status login(void);
        status connectToWiFi(const char* SSID, const char* PWD);
} server_t;

void loop_in_core2(void* pv_param);

#endif  // HYDRO_DB_H