#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"
#include "driver/ledc.h"

extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[]   asm("_binary_index_html_end");
extern const char chroma_start[] asm("_binary_chroma_png_start");
extern const char chroma_end[]   asm("_binary_chroma_png_end");


int32_t let_r = 0, let_g = 0, let_b = 0;

#define LEDC_TIMER          LEDC_TIMER_0            // timer que usaremos
#define LEDC_MODE           LEDC_HIGH_SPEED_MODE    // Mode de velocidad
#define LEDC_OUTPUT_IO_R    15                           // Pin de salida
#define LEDC_OUTPUT_IO_G    13                      // Pin de salida
#define LEDC_OUTPUT_IO_B    12                      // Pin de salida
#define LEDC_CHANNEL_R      LEDC_CHANNEL_0          // Canal de salida
#define LEDC_CHANNEL_G      LEDC_CHANNEL_1          // Canal de salida
#define LEDC_CHANNEL_B      LEDC_CHANNEL_2          // Canal de salida
#define LEDC_DUTY_RES       LEDC_TIMER_8_BIT       // Resolucion de la salida   
#define LEDC_FREQUENCY      5000                    // Frecuencia de la salida
#define LEDC_DUTY           0                       // Duty de la salida
#define LEDC_HPOINT         0                       // Ajuste de fase


static esp_err_t home_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, index_start, index_end - index_start);
    return ESP_OK;
}

static esp_err_t chroma_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "image/png");
    httpd_resp_send(req, chroma_start, chroma_end - chroma_start);
    return ESP_OK;
}

static esp_err_t api_get_handler(httpd_req_t *req) {
    printf("Updating RGB color\n");
    char* buf;
    size_t buf_len;
    buf_len = httpd_req_get_url_query_len(req) + 1;

    if(buf_len > 1) {
        buf = malloc(buf_len);
        if(httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            char param[10];
            if(httpd_query_key_value(buf, "r", param, sizeof(param)) == ESP_OK) {
                let_r = atoi(param);
                ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, let_r);
                ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);
            }
            if(httpd_query_key_value(buf, "g", param, sizeof(param)) == ESP_OK) {
                let_g = atoi(param);
                ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, let_g);
                ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);
            }
            if(httpd_query_key_value(buf, "b", param, sizeof(param)) == ESP_OK) {
                let_b = atoi(param);
                ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, let_b);
                ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);
            }
        }
        free(buf);
    }

    char res[30];
    httpd_resp_set_hdr(req, "Content-Type", "application/json");
    sprintf(res, "{\"r\":%ld,\"g\":%ld,\"b\":%ld}", let_r, let_g, let_b);

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, res, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t api = {
    .uri = "/api",
    .method = HTTP_GET,
    .handler = api_get_handler
};

static const httpd_uri_t home = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = home_get_handler
};

static const httpd_uri_t chroma = {
    .uri = "/chroma.png",
    .method = HTTP_GET,
    .handler = chroma_get_handler
};

void web_server_init(){
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
    if(httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &api);
        httpd_register_uri_handler(server, &home);
        httpd_register_uri_handler(server, &chroma);    
        return;
    } 

    printf("Server Error\n");
}


void app_main(void) {

    nvs_flash_init();

    esp_netif_init();
    esp_event_loop_create_default();

    example_connect();

    esp_netif_ip_info_t ip_info;
    esp_netif_t* netif = NULL;
    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if(netif == NULL) {
        printf("netif is NULL\n");
    } else {
        esp_netif_get_ip_info(netif, &ip_info);
        printf("IP address: %d.%d.%d.%d\n", IP2STR(&ip_info.ip));

        ledc_timer_config_t ledc_timer = {
            .duty_resolution = LEDC_DUTY_RES,
            .freq_hz = LEDC_FREQUENCY,
            .speed_mode = LEDC_MODE,
            .timer_num = LEDC_TIMER,
            .clk_cfg = LEDC_AUTO_CLK,
        };
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

        ledc_channel_config_t ledc_channel_r = {
            .channel    = LEDC_CHANNEL_R,
            .duty       = LEDC_DUTY,
            .gpio_num   = LEDC_OUTPUT_IO_R,
            .speed_mode = LEDC_MODE,
            .hpoint     = LEDC_HPOINT,
            .timer_sel  = LEDC_TIMER
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_r));

        ledc_channel_config_t ledc_channel_g = {
            .channel    = LEDC_CHANNEL_G,
            .duty       = LEDC_DUTY,
            .gpio_num   = LEDC_OUTPUT_IO_G,
            .speed_mode = LEDC_MODE,
            .hpoint     = LEDC_HPOINT,
            .timer_sel  = LEDC_TIMER
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_g));

        ledc_channel_config_t ledc_channel_b = {
            .channel    = LEDC_CHANNEL_B,
            .duty       = LEDC_DUTY,
            .gpio_num   = LEDC_OUTPUT_IO_B,
            .speed_mode = LEDC_MODE,
            .hpoint     = LEDC_HPOINT,
            .timer_sel  = LEDC_TIMER
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_b));

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, let_r);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, let_g);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, let_b);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);

        web_server_init();
    }


}
