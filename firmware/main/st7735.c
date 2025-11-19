#include "st7735.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "st7735";
static spi_device_handle_t spi;

// This function is called (in irq context!) just before a transmission starts.
// It will set the D/C line to the value indicated in the user field.
static void lcd_spi_pre_transfer_callback(spi_transaction_t *t) {
    int dc_level = (int)t->user;
    gpio_set_level(PIN_NUM_DC, dc_level);
}

// --- Low-level SPI communication functions ---
static void st7735_send_cmd(uint8_t cmd);
static void st7735_send_data(const uint8_t *data, int len);
static void st7735_reset(void);

/**
 * @brief Initialize the ST7735 display
 */
void st7735_init(void) {
    ESP_LOGI(TAG, "Initializing ST7735 display");

    // Configure GPIOs
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_BLK, GPIO_MODE_OUTPUT);

    // Reset the display
    st7735_reset();

    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = ST7735_WIDTH * ST7735_HEIGHT * 2,
    };

    // SPI device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 26 * 1000 * 1000, // 26 MHz
        .mode = 0,                         // SPI mode 0
        .spics_io_num = -1,                // CS pin not used
        .queue_size = 7,
        .pre_cb = lcd_spi_pre_transfer_callback, // Specify pre-transfer callback
    };

    // Initialize the SPI bus
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));

    // Initialization sequence for ST7735
    st7735_send_cmd(ST7735_SWRESET);
    vTaskDelay(pdMS_TO_TICKS(150));

    st7735_send_cmd(ST7735_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(255));

    st7735_send_cmd(ST7735_FRMCTR1);
    st7735_send_data((uint8_t[]){0x01, 0x2C, 0x2D}, 3);

    st7735_send_cmd(ST7735_FRMCTR2);
    st7735_send_data((uint8_t[]){0x01, 0x2C, 0x2D}, 3);

    st7735_send_cmd(ST7735_FRMCTR3);
    st7735_send_data((uint8_t[]){0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D}, 6);

    st7735_send_cmd(ST7735_INVCTR);
    st7735_send_data((uint8_t[]){0x07}, 1);

    st7735_send_cmd(ST7735_PWCTR1);
    st7735_send_data((uint8_t[]){0xA2, 0x02, 0x84}, 3);

    st7735_send_cmd(ST7735_PWCTR2);
    st7735_send_data((uint8_t[]){0xC5}, 1);

    st7735_send_cmd(ST7735_PWCTR3);
    st7735_send_data((uint8_t[]){0x0A, 0x00}, 2);

    st7735_send_cmd(ST7735_PWCTR4);
    st7735_send_data((uint8_t[]){0x8A, 0x2A}, 2);

    st7735_send_cmd(ST7735_PWCTR5);
    st7735_send_data((uint8_t[]){0x8A, 0xEE}, 2);

    st7735_send_cmd(ST7735_VMCTR1);
    st7735_send_data((uint8_t[]){0x0E}, 1);

    st7735_send_cmd(ST7735_INVOFF);

    st7735_send_cmd(ST7735_MADCTL);
    st7735_send_data((uint8_t[]){0xC8}, 1);

    st7735_send_cmd(ST7735_COLMOD);
    st7735_send_data((uint8_t[]){0x05}, 1); // 16-bit/pixel

    st7735_send_cmd(ST7735_CASET);
    st7735_send_data((uint8_t[]){0x00, 0x00, 0x00, 0x7F}, 4); // 128 pixels

    st7735_send_cmd(ST7735_RASET);
    st7735_send_data((uint8_t[]){0x00, 0x00, 0x00, 0x9F}, 4); // 160 pixels

    st7735_send_cmd(ST7735_GMCTRP1);
    st7735_send_data((uint8_t[]){0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10}, 16);

    st7735_send_cmd(ST7735_GMCTRN1);
    st7735_send_data((uint8_t[]){0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10}, 16);

    st7735_send_cmd(ST7735_NORON);
    vTaskDelay(pdMS_TO_TICKS(10));

    st7735_send_cmd(ST7735_DISPON);
    vTaskDelay(pdMS_TO_TICKS(100));

    // Turn on backlight
    gpio_set_level(PIN_NUM_BLK, 1);

    ESP_LOGI(TAG, "ST7735 display initialized");
}

// Helper function to set the drawing window
static void st7735_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    st7735_send_cmd(ST7735_CASET);
    uint8_t data[] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    st7735_send_data(data, 4);

    st7735_send_cmd(ST7735_RASET);
    uint8_t data2[] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
    st7735_send_data(data2, 4);

    st7735_send_cmd(ST7735_RAMWR);
}

void st7735_draw_pixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= ST7735_WIDTH || y < 0 || y >= ST7735_HEIGHT) {
        return;
    }
    st7735_set_address_window(x, y, x, y);
    uint8_t data[] = {color >> 8, color & 0xFF};
    st7735_send_data(data, 2);
}

void st7735_fill_screen(uint16_t color) {
    st7735_set_address_window(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1);

    int buffer_size = ST7735_WIDTH * 2; // Send one line at a time
    uint8_t *buffer = (uint8_t *)malloc(buffer_size);
    if (buffer == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for screen buffer");
        return;
    }

    // Fill the buffer with the color
    for (int i = 0; i < buffer_size; i += 2) {
        buffer[i] = color >> 8;
        buffer[i + 1] = color & 0xFF;
    }

    // Send the buffer to the screen line by line
    for (int y = 0; y < ST7735_HEIGHT; y++) {
        st7735_send_data(buffer, buffer_size);
    }

    free(buffer);
}

static void st7735_send_cmd(uint8_t cmd) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &cmd;
    t.user = (void *)0; // D/C low for command
    ret = spi_device_polling_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);
}

static void st7735_send_data(const uint8_t *data, int len) {
    esp_err_t ret;
    spi_transaction_t t;
    if (len == 0) {
        return;
    }
    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = data;
    t.user = (void *)1; // D/C high for data
    ret = spi_device_polling_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);
}

static void st7735_reset(void) {
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
}
