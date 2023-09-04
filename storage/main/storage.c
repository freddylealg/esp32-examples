#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "nvs.h"


void app_main(void)
{
    printf("Start\n");

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    err != ESP_OK ? printf("Error (%s) initializing NVS\n", esp_err_to_name(err)) : printf("NVS initialized\n");

    // Open NVS 
    nvs_handle_t storage_handle;
    err = nvs_open("storage", NVS_READWRITE, &storage_handle);
    err != ESP_OK ? printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err)) : printf("NVS handle opened\n");

    // Write to NVS
    int32_t value = 10;
    err = nvs_set_i32(storage_handle, "value", value);
    err != ESP_OK ? printf("Error (%s) writing!\n", esp_err_to_name(err)) : printf("Write success\n");

    // Commit written value
    nvs_commit(storage_handle);

    // leer de NVS
    int32_t value_read;
    err = nvs_get_i32(storage_handle, "value", &value_read);
    switch (err) {
        case ESP_OK:
            printf("value = %ld\n", value_read);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    // Close NVS
    nvs_close(storage_handle);

    printf("End\n");
}
