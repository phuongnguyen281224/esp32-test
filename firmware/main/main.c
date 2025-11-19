#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "st7735.h"

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing ST7735");
    st7735_init();

    ESP_LOGI(TAG, "Starting display test");

    while (true)
    {
        ESP_LOGI(TAG, "Filling screen with RED");
        st7735_fill_screen(RED);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(TAG, "Filling screen with GREEN");
        st7735_fill_screen(GREEN);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(TAG, "Filling screen with BLUE");
        st7735_fill_screen(BLUE);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
