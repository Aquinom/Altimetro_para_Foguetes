#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    float altitude;
    float velocity;
    float acceleration;

    float P[2][2];

    uint32_t last_timestamp_ms;

    bool initialized;
    
    float max_altitude;

} kalman_altitude_t;

void kalman_init(
    kalman_altitude_t *kf);

void kalman_reset(
    kalman_altitude_t *kf);

void kalman_update(
    kalman_altitude_t *kf,
    float measured_altitude,
    float measured_acceleration,
    uint32_t timestamp_ms);