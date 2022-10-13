#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "wifi.h"
#include "http.h"

void app_main(void)
{
    res_init();

    wifi_init();

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    printf("Waiting for 2 seconds lmao \r\n");

    post_function();
}
