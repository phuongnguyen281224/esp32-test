#ifndef ST7735_H
#define ST7735_H

#include "driver/spi_master.h"

// Pin configuration
#define PIN_NUM_SCLK CONFIG_SPI_SCLK_GPIO
#define PIN_NUM_MOSI CONFIG_SPI_MOSI_GPIO
#define PIN_NUM_MISO -1 // Not used
#define PIN_NUM_DC   CONFIG_ST7735_DC_GPIO
#define PIN_NUM_RST  CONFIG_ST7735_RST_GPIO
#define PIN_NUM_BLK  CONFIG_ST7735_BLK_GPIO

// Display resolution (1.8 inch ST7735)
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160

// ST7735 commands
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_MADCTL  0x36
#define ST7735_COLMOD  0x3A
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

void st7735_init(void);
void st7735_draw_pixel(int16_t x, int16_t y, uint16_t color);
void st7735_fill_screen(uint16_t color);

#endif // ST7735_H
