#include "servo.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdbool.h>

#define SERVO_GPIO             GPIO_NUM_14

#define LEDC_TIMER             LEDC_TIMER_0
#define LEDC_MODE              LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL           LEDC_CHANNEL_0
#define LEDC_DUTY_RES          LEDC_TIMER_16_BIT
#define LEDC_FREQUENCY         50

#define SERVO_MIN_PULSE_US     500
#define SERVO_MAX_PULSE_US     2500
#define SERVO_PERIOD_US        20000

#define SERVO_MAX_DEGREE       180

#define SERVO_CLOSED_ANGLE     0
#define SERVO_OPEN_ANGLE       90

static bool initialized = false;

static bool parachute_opened = false;

static const char *TAG = "SERVO";

static uint32_t angle_to_duty(int angle)
{
    if (angle < 0)
    {
        angle = 0;
    }

    if (angle > SERVO_MAX_DEGREE)
    {
        angle = SERVO_MAX_DEGREE;
    }

    uint32_t pulse_width =
        SERVO_MIN_PULSE_US +
        (
            angle *
            (
                SERVO_MAX_PULSE_US -
                SERVO_MIN_PULSE_US
            )
        ) /
        SERVO_MAX_DEGREE;

    return
        (pulse_width *
         (1 << LEDC_DUTY_RES))
        /
        SERVO_PERIOD_US;
}

void servo_init(void)
{
    if (initialized)
    {
        return;
    }

    ledc_timer_config_t timer =
    {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(
        ledc_timer_config(&timer));

    ledc_channel_config_t channel =
    {
        .gpio_num = SERVO_GPIO,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };

    ESP_ERROR_CHECK(
        ledc_channel_config(&channel));

    initialized = true;

    servo_close();
}

void servo_set_angle(int angle)
{
    if (!initialized)
    {
        return;
    }

    uint32_t duty =
        angle_to_duty(angle);

    ledc_set_duty(
        LEDC_MODE,
        LEDC_CHANNEL,
        duty);

    ledc_update_duty(
        LEDC_MODE,
        LEDC_CHANNEL);
}

void servo_open(void)
{
    if(parachute_opened)
    {
        return;
    }

    ESP_LOGI(TAG, "ABRINDO PARAQUEDAS");

    servo_set_angle(
        SERVO_OPEN_ANGLE);

    parachute_opened = true;
}

void servo_close(void)
{
    ESP_LOGI(TAG, "FECHANDO PARAQUEDAS");
    servo_set_angle(
        SERVO_CLOSED_ANGLE);
}