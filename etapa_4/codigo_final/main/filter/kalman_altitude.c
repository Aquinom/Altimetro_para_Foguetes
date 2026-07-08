#include "kalman_altitude.h"

#include <string.h>
#include <math.h>

#define KALMAN_Q_ALTITUDE      0.05f
#define KALMAN_Q_VELOCITY      0.10f

/*
 * Como utilizamos:
 *
 * BMP280 #1
 * BMP280 #2
 * BMP280 #3
 *
 * e fazemos mediana dos sensores,
 * podemos confiar mais na medição.
 */
#define KALMAN_R_ALTITUDE      0.8f

void kalman_init(kalman_altitude_t *kf)
{
    memset(kf, 0, sizeof(kalman_altitude_t));

    kf->P[0][0] = 1.0f;
    kf->P[1][1] = 1.0f;

    kf->initialized = false;
}

void kalman_reset(kalman_altitude_t *kf)
{
    kalman_init(kf);
}

void kalman_update(
    kalman_altitude_t *kf,
    float measured_altitude,
    float measured_acceleration,
    uint32_t timestamp_ms)
{
    if (!kf->initialized)
    {
        kf->altitude = measured_altitude;
        kf->velocity = 0.0f;
        kf->acceleration = measured_acceleration;
        kf->max_altitude = measured_altitude;
        kf->last_timestamp_ms = timestamp_ms;
        kf->initialized = true;
        return;
    }

    float dt =
        (timestamp_ms - kf->last_timestamp_ms) / 1000.0f;

    if (dt <= 0.0f)
    {
        return;
    }

    /*
     * Ignora amostras caso haja uma pausa longa
     * (breakpoint, reset parcial, scheduler etc.).
     */
    if (dt > 0.2f)
    {
        kf->last_timestamp_ms = timestamp_ms;
        return;
    }

    kf->last_timestamp_ms = timestamp_ms;

    /*
     * Predição
     */

    float altitude_pred =
        kf->altitude +
        kf->velocity * dt +
        0.5f * measured_acceleration * dt * dt;

    float velocity_pred =
        kf->velocity +
        measured_acceleration * dt;

    float P00 =
        kf->P[0][0] +
        dt * (kf->P[1][0] + kf->P[0][1]) +
        dt * dt * kf->P[1][1] +
        KALMAN_Q_ALTITUDE;

    float P01 =
        kf->P[0][1] +
        dt * kf->P[1][1];

    float P10 =
        kf->P[1][0] +
        dt * kf->P[1][1];

    float P11 =
        kf->P[1][1] +
        KALMAN_Q_VELOCITY;

    /*
     * Correção
     */

    float innovation =
        measured_altitude -
        altitude_pred;

    float S =
        P00 +
        KALMAN_R_ALTITUDE;

    if (S <= 0.0f)
    {
        return;
    }

    float K0 = P00 / S;
    float K1 = P10 / S;

    kf->altitude =
        altitude_pred +
        K0 * innovation;

    kf->velocity =
        velocity_pred +
        K1 * innovation;

    kf->acceleration =
        measured_acceleration;

    kf->P[0][0] =
        (1.0f - K0) * P00;

    kf->P[0][1] =
        (1.0f - K0) * P01;

    kf->P[1][0] =
        P10 -
        K1 * P00;

    kf->P[1][1] =
        P11 -
        K1 * P01;

    /*
     * Altitude máxima
     */

    if (kf->altitude > kf->max_altitude)
    {
        kf->max_altitude = kf->altitude;
    }
}