#pragma once

#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t g_flight_data_mutex;

typedef struct
{
    float pressure;

    float altitude;

    float filtered_altitude;

    float velocity;

    float acceleration;

} flight_data_t;


typedef struct
{
    bool peak_detected;

    float peak_altitude;

} flight_status_t;


extern flight_data_t g_flight_data;
extern flight_status_t g_flight_status;