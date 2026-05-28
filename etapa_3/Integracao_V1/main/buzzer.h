#ifndef BUZZER_H
#define BUZZER_H

#include "driver/gpio.h"

//====================================================
// BUZZER FUNCTIONS
//====================================================

void buzzer_init(gpio_num_t pin);

void buzzer_1bip(gpio_num_t pin);

void buzzer_doneInit(gpio_num_t pin);

void buzzer_stateBeep(gpio_num_t pin, const char *state);

#endif