/*
 * oled.c
 *
 *  Created on: Aug 12, 2018
 *      Author: nxp89240
 */

#include <afe_shield.h>

uint8_t txBuff[130];

lpi2c_master_config_t masterConfig;

extern volatile uint8_t capbutton_value;
extern volatile uint8_t swbutton_value;
extern volatile uint32_t pot_value;


static const uint8_t font[][5]  = {
								 0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
		                         0x00, 0x00, 0x5F, 0x00, 0x00, // !
		                         0x00, 0x03, 0x00, 0x03, 0x00, // "
		                         0x14, 0x3E, 0x14, 0x3E, 0x14, // #
		                         0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
		                         0x43, 0x33, 0x08, 0x66, 0x61, // %
		                         0x36, 0x49, 0x55, 0x22, 0x50, // &
		                         0x00, 0x05, 0x03, 0x00, 0x00, // '
		                         0x00, 0x1C, 0x22, 0x41, 0x00, // (
		                         0x00, 0x41, 0x22, 0x1C, 0x00, // )
		                         0x14, 0x08, 0x3E, 0x08, 0x14, // *
		                         0x08, 0x08, 0x3E, 0x08, 0x08, // +
		                         0x00, 0x50, 0x30, 0x00, 0x00, // ,
		                         0x08, 0x08, 0x08, 0x08, 0x08, // -
		                         0x00, 0x60, 0x60, 0x00, 0x00, // .
		                         0x20, 0x10, 0x08, 0x04, 0x02, // /
		                         //0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
								 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		                         0x04, 0x02, 0x7F, 0x00, 0x00, // 1
		                         0x42, 0x61, 0x51, 0x49, 0x46, // 2
		                         0x22, 0x41, 0x49, 0x49, 0x36, // 3
		                         0x18, 0x14, 0x12, 0x7F, 0x10, // 4
		                         0x27, 0x45, 0x45, 0x45, 0x39, // 5
		                         0x3E, 0x49, 0x49, 0x49, 0x32, // 6
		                         0x01, 0x01, 0x71, 0x09, 0x07, // 7
		                         0x36, 0x49, 0x49, 0x49, 0x36, // 8
		                         0x26, 0x49, 0x49, 0x49, 0x3E, // 9
		                         0x00, 0x36, 0x36, 0x00, 0x00, // :
		                         0x00, 0x56, 0x36, 0x00, 0x00, // ;
		                         0x08, 0x14, 0x22, 0x41, 0x00, // <
		                         0x14, 0x14, 0x14, 0x14, 0x14, // =
		                         0x00, 0x41, 0x22, 0x14, 0x08, // >
		                         0x02, 0x01, 0x51, 0x09, 0x06, // ?
		                         0x3E, 0x41, 0x59, 0x55, 0x5E, // @
		                         0x7E, 0x09, 0x09, 0x09, 0x7E, // A
		                         0x7F, 0x49, 0x49, 0x49, 0x36, // B
		                         0x3E, 0x41, 0x41, 0x41, 0x22, // C
		                         0x7F, 0x41, 0x41, 0x41, 0x3E, // D
		                         0x7F, 0x49, 0x49, 0x49, 0x41, // E
		                         0x7F, 0x09, 0x09, 0x09, 0x01, // F
		                         0x3E, 0x41, 0x41, 0x49, 0x3A, // G
		                         0x7F, 0x08, 0x08, 0x08, 0x7F, // H
		                         0x00, 0x41, 0x7F, 0x41, 0x00, // I
		                         0x30, 0x40, 0x40, 0x40, 0x3F, // J
		                         0x7F, 0x08, 0x14, 0x22, 0x41, // K
		                         0x7F, 0x40, 0x40, 0x40, 0x40, // L
		                         0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
		                         0x7F, 0x02, 0x04, 0x08, 0x7F, // N
		                         0x3E, 0x41, 0x41, 0x41, 0x3E, // O
		                         0x7F, 0x09, 0x09, 0x09, 0x06, // P
		                         0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
		                         0x7F, 0x09, 0x09, 0x09, 0x76,// R
								 0x26, 0x49, 0x49, 0x49, 0x32, // S
		                         0x01, 0x01, 0x7F, 0x01, 0x01, // T
		                         0x3F, 0x40, 0x40, 0x40, 0x3F, // U
		                         0x1F, 0x20, 0x40, 0x20, 0x1F, // V
		                         0x7F, 0x20, 0x10, 0x20, 0x7F, // W
		                         0x41, 0x22, 0x1C, 0x22, 0x41, // X
		                         0x07, 0x08, 0x70, 0x08, 0x07, // Y
		                         0x61, 0x51, 0x49, 0x45, 0x43, // Z
		                         0x00, 0x7F, 0x41, 0x00, 0x00, // [
		                         0x02, 0x04, 0x08, 0x10, 0x20, // '\'
		                         0x00, 0x00, 0x41, 0x7F, 0x00, // ]
		                         0x04, 0x02, 0x01, 0x02, 0x04, // ^
		                         0x40, 0x40, 0x40, 0x40, 0x40, // _
		                         0x00, 0x01, 0x02, 0x04, 0x00, // `
		                         0x20, 0x54, 0x54, 0x54, 0x78, // a
		                         0x7F, 0x44, 0x44, 0x44, 0x38, // b
		                         0x38, 0x44, 0x44, 0x44, 0x44, // c
		                         0x38, 0x44, 0x44, 0x44, 0x7F, // d
		                         0x38, 0x54, 0x54, 0x54, 0x18, // e
		                         0x04, 0x04, 0x7E, 0x05, 0x05, // f
		                         0x08, 0x54, 0x54, 0x54, 0x3C, // g
		                         0x7F, 0x08, 0x04, 0x04, 0x78, // h
		                         0x00, 0x44, 0x7D, 0x40, 0x00, // i
		                         0x20, 0x40, 0x44, 0x3D, 0x00, // j
		                         0x7F, 0x10, 0x28, 0x44, 0x00, // k
		                         0x00, 0x41, 0x7F, 0x40, 0x00, // l
		                         0x7C, 0x04, 0x78, 0x04, 0x78, // m
		                         0x7C, 0x08, 0x04, 0x04, 0x78, // n
		                         0x38, 0x44, 0x44, 0x44, 0x38, // o
		                         0x7C, 0x14, 0x14, 0x14, 0x08, // p
		                         0x08, 0x14, 0x14, 0x14, 0x7C, // q
		                         0x00, 0x7C, 0x08, 0x04, 0x04, // r
		                         0x48, 0x54, 0x54, 0x54, 0x20, // s
		                         0x04, 0x04, 0x3F, 0x44, 0x44, // t
		                         0x3C, 0x40, 0x40, 0x20, 0x7C, // u
		                         0x1C, 0x20, 0x40, 0x20, 0x1C, // v
		                         0x3C, 0x40, 0x30, 0x40, 0x3C, // w
		                         0x44, 0x28, 0x10, 0x28, 0x44, // x
		                         0x0C, 0x50, 0x50, 0x50, 0x3C, // y
		                         0x44, 0x64, 0x54, 0x4C, 0x44, // z
		                         0x00, 0x08, 0x36, 0x41, 0x41, // {
		                         0x00, 0x00, 0x7F, 0x00, 0x00, // |
		                         0x41, 0x41, 0x36, 0x08, 0x00, // }
		                         0x02, 0x01, 0x02, 0x04, 0x02};// ~


const uint8_t font12x16[][26]={

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char !
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char "
        0x00, 0x00, 0x00, 0x02, 0x10, 0x1E, 0x90, 0x1F, 0xF0, 0x1F, 0xFE, 0x03, 0x7E, 0x1E, 0x9E, 0x1F, 0xF0, 0x1F, 0xFE, 0x03, 0x7E, 0x02, 0x1E, 0x00, 0x10, 0x00,  // Code for char #
        0x00, 0x00, 0x00, 0x00, 0x78, 0x04, 0xFC, 0x0C, 0xFC, 0x0C, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xCC, 0x0F, 0xCC, 0x0F, 0x88, 0x07, 0x00, 0x00, 0x00, 0x00,  // Code for char $
        0x00, 0x00, 0x00, 0x30, 0x38, 0x38, 0x38, 0x3C, 0x38, 0x1E, 0x38, 0x0F, 0x80, 0x07, 0xC0, 0x03, 0xE0, 0x39, 0xF0, 0x38, 0x78, 0x38, 0x3C, 0x38, 0x1C, 0x00,  // Code for char %
        0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xB8, 0x3F, 0xFC, 0x3F, 0xFE, 0x31, 0xE6, 0x37, 0xFE, 0x3F, 0x3E, 0x1E, 0x1C, 0x3E, 0x00, 0x36, 0x00, 0x22, 0x00, 0x00,  // Code for char &
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char '
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x03, 0xFC, 0x0F, 0xFE, 0x1F, 0xFF, 0x3F, 0x07, 0x38, 0x01, 0x20, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char (
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x20, 0x01, 0x20, 0x07, 0x38, 0xFF, 0x3F, 0xFE, 0x1F, 0xFC, 0x0F, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char )
        0x00, 0x00, 0x00, 0x00, 0x98, 0x0C, 0xB8, 0x0E, 0xF8, 0x0F, 0xF8, 0x0F, 0xF8, 0x0F, 0xF8, 0x0F, 0xF8, 0x0F, 0xB8, 0x0E, 0x98, 0x0C, 0x00, 0x00, 0x00, 0x00,  // Code for char *
        0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00,  // Code for char +
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ,
        0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00,  // Code for char -
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char .
        0x00, 0x00, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x1E, 0x00, 0x0F, 0x80, 0x07, 0xC0, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x78, 0x00, 0x3C, 0x00, 0x1E, 0x00, 0x0E, 0x00,  // Code for char /
        0x00, 0x00, 0xF8, 0x07, 0xFE, 0x1F, 0xFE, 0x1F, 0x07, 0x3F, 0x83, 0x33, 0xC3, 0x31, 0xE3, 0x30, 0x73, 0x30, 0x3F, 0x38, 0xFE, 0x1F, 0xFE, 0x1F, 0xF8, 0x07,  // Code for char 0
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x0C, 0x30, 0x0E, 0x30, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00,  // Code for char 1
        0x00, 0x00, 0x1C, 0x30, 0x1E, 0x38, 0x1F, 0x3C, 0x07, 0x3E, 0x03, 0x3F, 0x83, 0x37, 0xC3, 0x33, 0xE3, 0x31, 0xF7, 0x30, 0x7F, 0x30, 0x3E, 0x30, 0x1C, 0x30,  // Code for char 2
        0x00, 0x00, 0x0C, 0x0C, 0x0E, 0x1C, 0x0F, 0x3C, 0xC7, 0x38, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xE7, 0x39, 0xFF, 0x3F, 0x7E, 0x1F, 0x3C, 0x0E,  // Code for char 3
        0x00, 0x00, 0xC0, 0x03, 0xE0, 0x03, 0xF0, 0x03, 0x78, 0x03, 0x3C, 0x03, 0x1E, 0x03, 0x0F, 0x03, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x03, 0x00, 0x03,  // Code for char 4
        0x00, 0x00, 0x3F, 0x0C, 0x7F, 0x1C, 0x7F, 0x3C, 0x63, 0x38, 0x63, 0x30, 0x63, 0x30, 0x63, 0x30, 0x63, 0x30, 0xE3, 0x38, 0xE3, 0x3F, 0xC3, 0x1F, 0x83, 0x0F,  // Code for char 5
        0x00, 0x00, 0xC0, 0x0F, 0xF0, 0x1F, 0xF8, 0x3F, 0xFC, 0x39, 0xDE, 0x30, 0xCF, 0x30, 0xC7, 0x30, 0xC3, 0x30, 0xC3, 0x39, 0xC3, 0x3F, 0x80, 0x1F, 0x00, 0x0F,  // Code for char 6
        0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x30, 0x03, 0x3C, 0x03, 0x3F, 0xC3, 0x0F, 0xF3, 0x03, 0xFF, 0x00, 0x3F, 0x00, 0x0F, 0x00, 0x03, 0x00,  // Code for char 7
        0x00, 0x00, 0x00, 0x0F, 0xBC, 0x1F, 0xFE, 0x3F, 0xFF, 0x39, 0xE7, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xE7, 0x30, 0xFF, 0x39, 0xFE, 0x3F, 0xBC, 0x1F, 0x00, 0x0F,  // Code for char 8
        0x00, 0x00, 0x3C, 0x00, 0x7E, 0x00, 0xFF, 0x30, 0xE7, 0x30, 0xC3, 0x30, 0xC3, 0x38, 0xC3, 0x3C, 0xC3, 0x1E, 0xE7, 0x0F, 0xFF, 0x07, 0xFE, 0x03, 0xFC, 0x00,  // Code for char 9
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x1C, 0x70, 0x1C, 0x70, 0x1C, 0x70, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x9C, 0x70, 0xFC, 0x70, 0xFC, 0x70, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
        0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0x01, 0xF0, 0x03, 0xF8, 0x07, 0x3C, 0x0F, 0x1E, 0x1E, 0x0F, 0x3C, 0x07, 0x38, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00,  // Code for char <
        0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x00, 0x00,  // Code for char =
        0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x07, 0x38, 0x0F, 0x3C, 0x1E, 0x1E, 0x3C, 0x0F, 0xF8, 0x07, 0xF0, 0x03, 0xE0, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char >
        0x00, 0x00, 0x1C, 0x00, 0x1E, 0x00, 0x1F, 0x00, 0x07, 0x00, 0x83, 0x37, 0xC3, 0x37, 0xE3, 0x37, 0xF7, 0x00, 0x7F, 0x00, 0x3E, 0x00, 0x1C, 0x00, 0x00, 0x00,  // Code for char ?
        0x00, 0x00, 0xF8, 0x0F, 0xFE, 0x1F, 0xFF, 0x1F, 0xF7, 0x3B, 0xFB, 0x37, 0xFB, 0x37, 0xFB, 0x37, 0xFB, 0x37, 0xFF, 0x37, 0xFF, 0x37, 0xFE, 0x03, 0xF8, 0x01,  // Code for char @
        0x00, 0x00, 0x00, 0x38, 0x00, 0x3F, 0xE0, 0x3F, 0xFC, 0x07, 0xFF, 0x06, 0x1F, 0x06, 0xFF, 0x06, 0xFC, 0x07, 0xE0, 0x3F, 0x00, 0x3F, 0x00, 0x38, 0x00, 0x00,  // Code for char A
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xE7, 0x30, 0xFF, 0x39, 0xFE, 0x3F, 0xBC, 0x1F, 0x00, 0x0F, 0x00, 0x00,  // Code for char B
        0x00, 0x00, 0xF0, 0x03, 0xFC, 0x0F, 0xFE, 0x1F, 0x0F, 0x3C, 0x07, 0x38, 0x03, 0x30, 0x03, 0x30, 0x07, 0x38, 0x0F, 0x3C, 0x0E, 0x1C, 0x0C, 0x0C, 0x00, 0x00,  // Code for char C
		0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x03, 0x30, 0x03, 0x30, 0x03, 0x30, 0x07, 0x38, 0x0F, 0x3C, 0xFE, 0x1F, 0xFC, 0x0F, 0xF0, 0x03, 0x00, 0x00,  // Code for char D
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0x03, 0x30, 0x03, 0x30, 0x00, 0x00,  // Code for char E
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00,  // Code for char F
        0x00, 0x00, 0xF0, 0x03, 0xFC, 0x0F, 0xFE, 0x1F, 0x0F, 0x3C, 0x07, 0x38, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC7, 0x3F, 0xC7, 0x3F, 0xC6, 0x3F, 0x00, 0x00,  // Code for char G
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x00,  // Code for char H
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x03, 0x30, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x03, 0x30, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char I
        0x00, 0x00, 0x00, 0x0E, 0x00, 0x1E, 0x00, 0x3E, 0x00, 0x38, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x38, 0xFF, 0x3F, 0xFF, 0x1F, 0xFF, 0x07, 0x00, 0x00,  // Code for char J
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xE0, 0x01, 0xF0, 0x03, 0xF8, 0x07, 0x3C, 0x0F, 0x1E, 0x1E, 0x0F, 0x3C, 0x07, 0x38, 0x03, 0x30, 0x00, 0x00,  // Code for char K
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00,  // Code for char L
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x7E, 0x00, 0xF8, 0x01, 0xE0, 0x01, 0xF8, 0x01, 0x7E, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x00,  // Code for char M
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x3E, 0x00, 0xF8, 0x00, 0xF0, 0x03, 0xC0, 0x07, 0x00, 0x1F, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x00,  // Code for char N
        0x00, 0x00, 0xF0, 0x03, 0xFC, 0x0F, 0xFE, 0x1F, 0x0F, 0x3C, 0x07, 0x38, 0x03, 0x30, 0x07, 0x38, 0x0F, 0x3C, 0xFE, 0x1F, 0xFC, 0x0F, 0xF0, 0x03, 0x00, 0x00,  // Code for char O
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x83, 0x01, 0x83, 0x01, 0x83, 0x01, 0x83, 0x01, 0xC7, 0x01, 0xFF, 0x01, 0xFE, 0x00, 0x7C, 0x00, 0x00, 0x00,  // Code for char P
        0x00, 0x00, 0xF0, 0x03, 0xFC, 0x0F, 0xFE, 0x1F, 0x0F, 0x3C, 0x07, 0x38, 0x03, 0x36, 0x07, 0x3E, 0x0F, 0x3E, 0xFE, 0x3F, 0xFC, 0x3F, 0xF0, 0x33, 0x00, 0x00,  // Code for char Q
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x83, 0x01, 0x83, 0x03, 0x83, 0x07, 0x83, 0x0F, 0xC7, 0x1F, 0xFF, 0x3D, 0xFE, 0x38, 0x7C, 0x30, 0x00, 0x00,  // Code for char R
		0x00, 0x00, 0x3C, 0x0C, 0x7E, 0x1C, 0xFF, 0x3C, 0xE7, 0x38, 0xC3, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC7, 0x39, 0xCF, 0x3F, 0x8E, 0x1F, 0x0C, 0x0F, 0x00, 0x00,  // Code for char S
        0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char T
        0x00, 0x00, 0xFF, 0x07, 0xFF, 0x1F, 0xFF, 0x3F, 0x00, 0x38, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x38, 0xFF, 0x3F, 0xFF, 0x1F, 0xFF, 0x07, 0x00, 0x00,  // Code for char U
        0x00, 0x00, 0x07, 0x00, 0x3F, 0x00, 0xFF, 0x01, 0xF8, 0x0F, 0xC0, 0x3F, 0x00, 0x3E, 0xC0, 0x3F, 0xF8, 0x0F, 0xFF, 0x01, 0x3F, 0x00, 0x07, 0x00, 0x00, 0x00,  // Code for char V
        0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x1E, 0x80, 0x07, 0x80, 0x03, 0x80, 0x07, 0x00, 0x1E, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x00,  // Code for char W
        0x00, 0x00, 0x03, 0x30, 0x0F, 0x3C, 0x1F, 0x3E, 0x3C, 0x0F, 0xF0, 0x03, 0xE0, 0x01, 0xF0, 0x03, 0x3C, 0x0F, 0x1F, 0x3E, 0x0F, 0x3C, 0x03, 0x30, 0x00, 0x00,  // Code for char X
        0x00, 0x00, 0x03, 0x00, 0x0F, 0x00, 0x3F, 0x00, 0xFC, 0x00, 0xF0, 0x3F, 0xC0, 0x3F, 0xF0, 0x3F, 0xFC, 0x00, 0x3F, 0x00, 0x0F, 0x00, 0x03, 0x00, 0x00, 0x00,  // Code for char Y
        0x00, 0x00, 0x03, 0x30, 0x03, 0x3C, 0x03, 0x3E, 0x03, 0x3F, 0xC3, 0x33, 0xE3, 0x31, 0xF3, 0x30, 0x3F, 0x30, 0x1F, 0x30, 0x0F, 0x30, 0x03, 0x30, 0x00, 0x00,  // Code for char Z
		};

const uint8_t logo_app[] = {

	0x80, 0x28,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFE, 0xF8, 0xE3, 0xC7, 0x1F, 0x7F, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xE0, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x8F, 0xE3, 0xF1, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xE3,
	0x87, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
	0xF0, 0xF0, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
	0x3F, 0x8F, 0xC3, 0xF1, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x1F,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFE, 0xF8, 0xF0, 0xE0, 0xC0, 0xFF, 0xDF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x7C, 0x18, 0x83, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0x01, 0x3C,
	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0,
	0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFC,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x8F,
	0xE3, 0xF0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
	0x1F, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
	0xF8, 0xE1, 0xC7, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x03, 0x07, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x8F, 0xE3, 0xF1, 0xFC, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE3, 0xC7, 0x1F, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

const uint8_t logo_wifi_90x40[] = {
	0x5A, 0x28,
	0x00, 0x00, 0x80, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x02,
	0x06, 0x06, 0x0C, 0x08, 0x18, 0x30, 0xE0, 0x80, 0x00, 0x00, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xF1, 0x01, 0x01, 0x01, 0x03, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F, 0x07, 0x01, 0x01, 0x01,
	0x01, 0x07, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0xF1, 0xFF, 0xFF, 0xE3, 0xC1,
	0xC1, 0xC1, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x00,
	0x00, 0x00, 0x1C, 0x3E, 0x3E, 0x3E, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x3F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00,
	0x00, 0x00, 0x0F, 0x3F, 0x03, 0x00, 0x00, 0xE0, 0xFE, 0xFE, 0xE0, 0x00, 0x00, 0x03, 0x3F, 0x1F,
	0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C,
	0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x1F, 0x7F,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xC0, 0xC0, 0xC0, 0xC0, 0xF0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF0, 0xC0, 0xC0, 0xC0, 0xC0, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0x1F, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F,
	0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xBF, 0x9F,
	0x9F, 0x8F, 0x87, 0x83, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0x40, 0x40, 0x60, 0x30, 0x30, 0x18, 0x0C, 0x07, 0x01,
	0x00, 0x00
	};


const uint8_t logo_temperature_40x40[] = {
	0x28, 0x28,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFC, 0x3E, 0x0E, 0x06, 0x87,
	0x87, 0x07, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xFC,
	0xFF, 0x07, 0x03, 0x01, 0x00, 0xC0, 0xF0, 0xFF, 0xFF, 0xF0, 0xE0, 0x00, 0x00, 0x01, 0x03, 0x0F,
	0xFE, 0xF8, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x1E, 0x38, 0x70, 0x60, 0xE0, 0xE1, 0xE3,
	0xC3, 0xE1, 0xE0, 0x60, 0x70, 0x30, 0x3C, 0x1F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};


const uint8_t degC_20x16[] = {
	0x14, 0x10,
	0x7C, 0xFE, 0xC7, 0xC7, 0xC7, 0xFE, 0x7E, 0x38, 0x00, 0xF0, 0xFC, 0xFE, 0x0E, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x06, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1F, 0x3F,
	0x38, 0x70, 0x70, 0x70, 0x70, 0x70, 0x30, 0x38,
};

const uint8_t Per_20x16[] = {
	0x14, 0x10,
	0x00, 0x7C, 0xFE, 0x83, 0x83, 0xC7, 0xFE, 0x7C, 0x00, 0xC0, 0xF0, 0x3C, 0x8E, 0x83, 0xC0, 0xC0,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x61, 0x70, 0x1C, 0x07, 0x03, 0x00, 0x1E,
	0x3F, 0x61, 0x60, 0x60, 0x3F, 0x1F, 0x00, 0x00
};

const uint8_t logo_wifi_online_18x16[] = {
	0x12, 0x10,
	0x60, 0xF0, 0x78, 0x3C, 0x1C, 0x1E, 0x8E, 0x8E, 0x8E, 0x8E, 0x8E, 0x8E, 0x1E, 0x1C, 0x3C, 0x78,
	0xF0, 0x60, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0F, 0x07, 0x23, 0x73, 0x73, 0x23, 0x07, 0x0F, 0x06,
	0x02, 0x00, 0x00, 0x00
};

const uint8_t logo_wifi_offline_18x16[] = {
	0x12, 0x10,
	0x60, 0xF0, 0x78, 0x3C, 0x1C, 0x1E, 0x8E, 0x8E, 0x8E, 0x8E, 0xCE, 0x6E, 0x3E, 0x1C, 0x2C, 0x76,
	0xF3, 0x61, 0x40, 0x60, 0x30, 0x1A, 0x0E, 0x0F, 0x07, 0x23, 0x73, 0x71, 0x22, 0x07, 0x0F, 0x06,
	0x02, 0x00, 0x00, 0x00,
};

const uint8_t logo_cloud_offline_18x16[] = {
	0x12, 0x10,
	0x00, 0x80, 0xC0, 0xC0, 0x70, 0x10, 0x30, 0x30, 0xB8, 0x18, 0x0C, 0x8C, 0x18, 0x38, 0xF0, 0xC0,
	0x80, 0x00, 0x0F, 0x0F, 0x18, 0x10, 0x10, 0x10, 0x10, 0x15, 0x17, 0x13, 0x13, 0x17, 0x15, 0x10,
	0x10, 0x18, 0x0B, 0x0F
};

const uint8_t logo_cloud_online_18x16[] = {
	0x12, 0x10,
	0x00, 0x80, 0xC0, 0xC0, 0x70, 0x10, 0x30, 0x30, 0x38, 0x18, 0x0C, 0x0C, 0x18, 0x38, 0xF0, 0xC0,
	0x80, 0x00, 0x0F, 0x0F, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x18, 0x0B, 0x0F
};

const uint8_t logo_settings_40x40[]={
	0x28,0x28,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0,
	0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0,
	0xF0, 0xF0, 0xF0, 0xE0, 0xF7, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x3F, 0x1E, 0x3E, 0x3F, 0x3F, 0x7F,
	0xFF, 0xFF, 0xFF, 0xF7, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0xC3, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7,
	0xC7, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07,
	0x0F, 0x0F, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0x78, 0x7C, 0xFC, 0xFC, 0xFE,
	0xFF, 0xFF, 0xFF, 0xC7, 0x07, 0x07, 0x0F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03,
	0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t logo_humidity_40x40[] = {
	0x28,0x28,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3E, 0xFC, 0xF8,
	0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x7E, 0xFC,
	0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x01, 0x83, 0xFF, 0xFF, 0xFE, 0xF8, 0xC0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void OLED_Init(){

	I2C_Init();

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_DISPLAYOFF;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETDISPLAYCLOCKDIV;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x80;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETMULTIPLEX;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_LCDHEIGHT-2;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);


	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETDISPLAYOFFSET;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x00;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETSTARTLINE;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_CHARGEPUMP;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x14;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_MEMORYMODE;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x00;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SEGREMAP | 0x1;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_COMSCANDEC;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETCOMPINS;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x12; //?
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETCONTRAST;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x8F; //?
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_SETPRECHARGE;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0xF1;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x40;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_DISPLAYALLON_RESUME;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_NORMALDISPLAY;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_DEACTIVATE_SCROLL;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_DISPLAYON;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_COLUMNADDR;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x00;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_LCDWIDTH-1;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = SSD1306_PAGEADDR;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x00;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

	txBuff[0] = 0x00;
	txBuff[1] = 0x07;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);



}


void OLED_Clear_Screen(){

	memset(txBuff,0,129);
	txBuff[0] = 0x40;
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
	I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,129);
}


void OLED_PrintText(uint8_t line,uint8_t x,uint8_t data[]){

	uint8_t i,j,len,cmax;

	len =strlen(data);

	if( line<8){

		if((x + len*6) > 127){

			cmax=127;
		}
		else
		{
			cmax = x + len*6;
		}

		txBuff[0] = 0x00;
		txBuff[1] = SSD1306_COLUMNADDR;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = x;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = cmax;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = SSD1306_PAGEADDR;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = line;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = line;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x40;

		for(j=0;j<len;j++){
			for(i=0;i<5;i++){
				txBuff[j*6 + i +1] = font[data[j]-32][i];
			}
		}

		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,cmax-x +1);

	}
}


void OLED_PrintLargeText(uint8_t line,uint8_t x,uint8_t data[]){

	uint8_t i,j,k,len,cmax;

	len =strlen(data);

	if( line<7){

		if((x + len*13) > 127){

			cmax=127;
		}
		else
		{
			cmax = x + len*13;
		}

		txBuff[0] = 0x00;
		txBuff[1] = SSD1306_COLUMNADDR;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = x;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = cmax;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = SSD1306_PAGEADDR;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

		txBuff[0] = 0x00;
		txBuff[1] = line;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);
		txBuff[0] = 0x00;
		txBuff[1] = line;
		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);
		txBuff[0] = 0x40;

		for(j=0;j<len;j++){
			for(i=0,k=0;i<26;i=i+2,k++){
				txBuff[j*13 + k +1] = font12x16[data[j]-32][i];
			}
		}

		I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,cmax-x +1);

			txBuff[0] = 0x00;
			txBuff[1] = SSD1306_COLUMNADDR;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = x;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = cmax;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = SSD1306_PAGEADDR;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = line+1;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = line+1;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);
			txBuff[0] = 0x40;

			for(j=0;j<len;j++){
				for(i=1,k=0;i<26;i=i+2,k++){
					txBuff[j*13 + k +1] = font12x16[data[j]-32][i];
				}
			}
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,cmax-x +1);
	}
}


void OLED_DrawImage(uint8_t line, uint8_t x, uint8_t *logo){

	uint8_t i,j,len,cmax;
	uint32_t k;
	uint8_t size = logo[0];
	uint8_t hsize = logo[1]/8;

	if(x + size >128){

		x = 128 - size;
	}

	if( line + hsize <= 8){
		k=2;
		for(j=0;j<hsize;j++){
			txBuff[0] = 0x00;
			txBuff[1] = SSD1306_COLUMNADDR;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = x;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = x + size-1;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = SSD1306_PAGEADDR;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = line+j;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x00;
			txBuff[1] = line+j;
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,2);

			txBuff[0] = 0x40;
			for(i=0;i<size;i++,k++){
				txBuff[i +1] = logo[k];
			}
			I2C_WriteBuffer(SSD1306_I2C_ADDRESS, txBuff,size + 1);
		}



	}
}


uint32_t AFE_ReadData(){
	uint32_t capval;
	uint8_t rxbuffer[24];
	uint8_t txbuffer[2];
	status_t ret;

	txbuffer[0]=0x00;

	ret = I2C_WriteBuffer(0x42, txbuffer, 1);

	ret = I2C_ReadBuffer(0x42, 0x00,rxbuffer, 23);

	capbutton_value = rxbuffer[6] & 0xF;
	swbutton_value = rxbuffer[6]>>4 & 0x3;

	pot_value |= rxbuffer[22];
	pot_value<<=8;
	pot_value |=rxbuffer[21];
	pot_value<<=8;
	pot_value |=rxbuffer[20];
	pot_value<<=8;
	pot_value |=rxbuffer[19];

	return capval;

}


void I2C_Init()
{

	status_t reVal        = kStatus_Fail;
    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = LPI2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);



}


status_t I2C_WriteBuffer(uint16_t address ,uint8_t *buffer, uint16_t length ){

	status_t reVal        = kStatus_Fail;
    /* Send master blocking data to slave */
    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, address, kLPI2C_Write))
    {

        reVal = LPI2C_MasterSend(EXAMPLE_I2C_MASTER, buffer, length);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }

}

status_t I2C_ReadBuffer(uint16_t address ,uint16_t subaddress,uint8_t *buffer, uint16_t length ){

	status_t reVal        = kStatus_Fail;
    /* Receive blocking data from slave */
    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, address, kLPI2C_Read))
    {



 //       reVal = LPI2C_MasterRepeatedStart(EXAMPLE_I2C_MASTER, address, kLPI2C_Read);
 //       if (reVal != kStatus_Success)
 //       {
 //           return -1;
 //       }

        reVal = LPI2C_MasterReceive(EXAMPLE_I2C_MASTER, buffer, length );
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }

}