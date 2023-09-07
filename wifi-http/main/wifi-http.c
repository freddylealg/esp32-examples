#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"

static esp_err_t api_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, "Hello World", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t api = {
    .uri = "/api",
    .method = HTTP_GET,
    .handler = api_get_handler
};

void web_server_init(){
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
    if(httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &api);
        return;
    } 

    printf("Server Error\n");
}

void app_main(void) {
    int connected = 0;
    nvs_flash_init();

    esp_netif_init();
    esp_event_loop_create_default();

    example_connect();

    esp_netif_ip_info_t ip_info;
    esp_netif_t* netif = NULL;
    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    while(1) {
        printf("Checking connection...%i\n", connected);
        if(netif == NULL) {
            printf("netif is NULL\n");
        } else {
            
            printf("netif is not NULL\n");
            esp_netif_get_ip_info(netif, &ip_info);
            printf("IP address: %d.%d.%d.%d\n", IP2STR(&ip_info.ip));
            if( connected == 0 ){
                 web_server_init();
                 connected = 1;
            }
           
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}
