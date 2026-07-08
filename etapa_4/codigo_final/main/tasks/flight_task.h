#pragma once

#include <stdbool.h>

void flight_task_start(void);

void flight_task_delete(void);

bool flight_peak_detected(void);

float flight_peak_altitude(void);