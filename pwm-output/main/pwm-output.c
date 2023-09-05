#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"

#define LEDC_TIMER          LEDC_TIMER_0            // timer que usaremos
#define LEDC_MODE           LEDC_HIGH_SPEED_MODE    // Mode de velocidad
#define LEDC_OUTPUT         2                      // Pin de salida
#define LEDC_CHANNEL        LEDC_CHANNEL_0          // Canal de salida
#define LEDC_DUTY_RES       LEDC_TIMER_12_BIT       // Resolucion de la salida   
#define LEDC_FREQUENCY      5000                    // Frecuencia de la salida
#define LEDC_DUTY           0                       // Duty de la salida
#define LEDC_HPOINT         0                       // Ajuste de fase

void app_main(void) {

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,       
        .speed_mode = LEDC_MODE,         
        .timer_num = LEDC_TIMER,         
        .clk_cfg = LEDC_AUTO_CLK,       
    };
    ledc_timer_config(&ledc_timer);                 
    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL,                    
        .duty = LEDC_DUTY,                          
        .gpio_num = LEDC_OUTPUT,                    
        .speed_mode = LEDC_MODE,                    
        .hpoint = LEDC_HPOINT,                      
        .timer_sel = LEDC_TIMER,                    
        .intr_type = LEDC_INTR_DISABLE,
    };
    ledc_channel_config(&ledc_channel);

    while(1) {
        int raw_value;
        adc2_config_channel_atten(ADC2_CHANNEL_4, ADC_ATTEN_DB_0);
        
        while(1){
            adc2_get_raw(ADC2_CHANNEL_4, ADC_WIDTH_BIT_12, &raw_value);
            printf("ADC value: %d\n", raw_value);

            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, raw_value);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

}
