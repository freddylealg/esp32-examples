#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t taskHandle1 = NULL;
TaskHandle_t taskHandle2 = NULL;

void task1(void *args){
    int count = 0;
    while(1){
        count++;
        printf("Task 1: %i\n", count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if(count == 10){
            vTaskSuspend(taskHandle2);
        }
        if(count == 20){
            vTaskResume(taskHandle2);
        }
    }
}

void task2(void *args){
    int count = 0;
    while(1){
        count++;
        printf("Task 2: %i\n", count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void){

    xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 1, &taskHandle1, 0);
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 1, &taskHandle2, 1);


}
