#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED 12
#define BLINK_PERIOD 1000 //ms


void app_main(void) {
    int let_state = 0;
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(LED, let_state);
        let_state = !let_state;
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS);
    }

}
