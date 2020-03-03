/*
 * oled.h
 *
 *  Created on: Aug 12, 2018
 *      Author: nxp89240
 */

#ifndef AFE_SHIELD_H_
#define AFE_SHIELD_H_

#include "board.h"
#include "fsl_lpi2c.h"

/* 7-bit I2C addresses of I/O expander */
/* Note: The ROM code requires the address to be between bits [6:0]
         bit 7 is ignored */

#define EXAMPLE_I2C_MASTER_BASE (LPI2C1_BASE)

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY
#define WAIT_TIME 10U


#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)

#define LPI2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define LPI2C_BAUDRATE 100000U
#define LPI2C_DATA_LENGTH 33U

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_I2C_ADDRESS				0x3C

#define SSD1306_LCDWIDTH				128
#define SSD1306_LCDHEIGHT				64

#define SSD1306_SETCONTRAST				0x81
#define SSD1306_DISPLAYALLON_RESUME		0xA4
#define SSD1306_DISPLAYALLON			0xA5
#define SSD1306_NORMALDISPLAY			0xA6
#define SSD1306_INVERTDISPLAY			0xA7
#define SSD1306_DISPLAYOFF				0xAE
#define SSD1306_DISPLAYON				0xAF

#define SSD1306_SETDISPLAYOFFSET		0xD3
#define SSD1306_SETCOMPINS				0xDA
#define SSD1306_SETVCOMDETECT			0xDB
#define SSD1306_SETDISPLAYCLOCKDIV		0xD5
#define SSD1306_SETPRECHARGE			0xD9
#define SSD1306_SETMULTIPLEX			0xA8
#define SSD1306_SETLOWCOLUMN			0x00
#define SSD1306_SETHIGHCOLUMN			0x10
#define SSD1306_SETSTARTLINE			0x40

#define SSD1306_MEMORYMODE				0x20
#define SSD1306_COLUMNADDR				0x21
#define SSD1306_PAGEADDR				0x22
#define SSD1306_COMSCANINC				0xC0
#define SSD1306_COMSCANDEC				0xC8
#define SSD1306_SEGREMAP				0xA0
#define SSD1306_CHARGEPUMP				0x8D

#define SSD1306_EXTERNALVCC				0x01
#define SSD1306_SWITCHCAPVCC			0x02

#define SSD1306_ACTIVATE_SCROLL			0x2F
#define SSD1306_DEACTIVATE_SCROLL		0x2E
#define SSD1306_LEFT_HORIZONTAL_SCROLL	0x27
#define SSD1306_RIGHT_HORIZONTAL_SCROLL	0x26
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL		0x2A



extern const uint8_t logo_app[];
extern const uint8_t logo_wifi_90x40[];
extern const uint8_t logo_temperature_40x40[];
extern const uint8_t logo_humidity_40x40[];
extern const uint8_t logo_settings_40x40[];
extern const uint8_t degC_20x16[];
extern const uint8_t Per_20x16[];
extern const uint8_t logo_wifi_online_18x16[];
extern const uint8_t logo_wifi_offline_18x16[];
extern const uint8_t logo_cloud_online_18x16[];
extern const uint8_t logo_cloud_offline_18x16[];

uint32_t Pot_Read();
uint32_t AFE_ReadData();
status_t I2C_ReadBuffer(uint16_t address ,uint16_t subaddress,uint8_t *buffer, uint16_t length );
status_t I2C_WriteBuffer(uint16_t address ,uint8_t *buffer, uint16_t length);
void I2C_Init();
void OLED_Init();
void OLED_PrintText(uint8_t line,uint8_t x,uint8_t data[]);
void OLED_PrintLargeText(uint8_t line,uint8_t x,uint8_t data[]);
void OLED_DrawImage(uint8_t line, uint8_t x, uint8_t *logo);
#endif /* AFE_SHIELD_H_ */
