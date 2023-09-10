#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

void app_main(void) {

    while(1) {
        int raw_value;
        adc2_config_channel_atten(ADC2_CHANNEL_4, ADC_ATTEN_DB_0);
        
        while(1){
            adc2_get_raw(ADC2_CHANNEL_4, ADC_WIDTH_BIT_12, &raw_value);
            printf("ADC value: %d\n", raw_value);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

}
