#ifndef BUZZER_H
#define BUZZER_H

#include "driver/gpio.h"

void buzzer_1bip(gpio_num_t pin);

void buzzer_DoneInit(gpio_num_t pin);
#endif