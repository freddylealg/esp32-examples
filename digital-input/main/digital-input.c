#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define LED 12
#define BUTTON 14

void app_main(void) {
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    gpio_set_level(LED, 0);

    while(1) {
        if (gpio_get_level(BUTTON) == 1) {
            printf("Button pressed!\n");
            gpio_set_level(LED, 1);
        } else {
            gpio_set_level(LED, 0);
        }
    }

}
