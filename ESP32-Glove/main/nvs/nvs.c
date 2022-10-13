#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

void res_init()
{
    esp_err_t ret = nvs_flash_init();

    // Clearing Flash to help free up pages for nvs
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
}