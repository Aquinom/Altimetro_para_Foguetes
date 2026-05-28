#ifndef FAKETASKS_H
#define FAKETASKS_H

#include <stdbool.h>

//====================================================
// GLOBAL VARIABLES
//====================================================

extern volatile bool accelerometer_enabled;
extern volatile bool barometer_enabled;

extern volatile int accelerometer_value;
extern volatile int barometer_value;

//====================================================
// TASKS
//====================================================

void accelerometer_task(void *pv);

void barometer_task(void *pv);

void peak_detection_task(void *pv);

#endif