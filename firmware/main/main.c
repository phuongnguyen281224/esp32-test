#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"

static const char *TAG = "esp32_test";

void app_main(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);

    ESP_LOGI(TAG, "Hello from ESP32-S3!");
    ESP_LOGI(TAG, "Model: %s", chip_info.model == CHIP_ESP32S3 ? "ESP32-S3" : "Other");
    ESP_LOGI(TAG, "Cores: %d", chip_info.cores);
    ESP_LOGI(TAG, "Revision: %d", chip_info.revision);
    ESP_LOGI(TAG, "Flash: %uMB", flash_size / (1024 * 1024));

    while (true)
    {
        ESP_LOGI(TAG, "ESP-IDF is running inside Docker!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
