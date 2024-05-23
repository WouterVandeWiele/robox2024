/*!
 * @file SED1530_LCD
 *
 * GLCD SED1531 library License:
 * 
 *  Target independent implementation of the GLCD SED1531 library.
 *  The dependent mappings of the I/O pins must be included before this
 *  file.
 *
 *  Copyright 2013 Peter van Merkerk
 *
 *  This file is part of the GLCD SED1531 library.
 *
 *  The GLCD SED1531 library is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  The GLCD SED1531 library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  the GLCD SED1531 library If not, see http://www.gnu.org/licenses/.
 * 
 * 
 * Adafruit License:
 *
 * This is part of for Adafruit's GFX library, supplying generic support
 * for grayscale OLED displays: http://www.adafruit.com/category/63_98
 *
 * These displays use I2C or SPI to communicate. I2C requires 2 pins
 * (SCL+SDA) and optionally a RESET pin. SPI requires 4 pins (MOSI, SCK,
 * select, data/command) and optionally a reset pin. Hardware SPI or
 * 'bitbang' software SPI are both supported.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * BSD license, all text above, and the splash screen header file,
 * must be included in any redistribution.
 *
 */

#ifndef _SED1530_LCD_H_
#define _SED1530_LCD_H_

#if !defined(__AVR_ATtiny85__) // Not for ATtiny, at all

#include <stdint.h>
#include "Adafruit_GFX.h"
#include "robox_io.h"
// #include <Adafruit_GFX.h>
// #include <Adafruit_I2CDevice.h>
// #include <Adafruit_SPIDevice.h>
// #include <SPI.h>
// #include <Wire.h>
#include "general_config.h"

/* GLCD Commands */
#define GLCD_CMD_DISPLAY_OFF                    (0xAE)
#define GLCD_CMD_DISPLAY_ON                     (0xAF)
#define GLCD_CMD_INITIAL_DISPLAY_LINE(start)    (0x40 | (start))
#define GLCD_CMD_SET_PAGE(page)                 (0xB0 | (page))
#define GLCD_CMD_SET_COLUMN_HIGH(col_high)      (0x10 | (col_high))     
#define GLCD_CMD_SET_COLUMN_LOW(col_low)        (col_low)
#define GLCD_CMD_ADC_NORMAL                     (0xA0)     
#define GLCD_CMD_ADC_REVERSE                    (0xA1)     
#define GLCD_CMD_READ_MODIFY_WRITE              (0xE0)
#define GLCD_CMD_DISPLAY_NORMAL                 (0xA6)
#define GLCD_CMD_DISPLAY_REVERSE                (0xA7)
#define GLCD_CMD_ENTIRE_DISPLAY_NORMAL          (0xA4)
#define GLCD_CMD_ENTIRE_DISPLAY_ON              (0xA5)
#define GLCD_CMD_LCD_BIAS_1_8                   (0xA2)
#define GLCD_CMD_LCD_BIAS_1_6                   (0xA3)
#define GLCD_CMD_READ_MODIFY_WRITE              (0xE0)
#define GLCD_CMD_END                            (0xEE)
#define GLCD_CMD_RESET                          (0xE2)
#define GLCD_CMD_POWER_NONE                     (0x28)
#define GLCD_CMD_POWER_FOLLOWER                 (0x29)
#define GLCD_CMD_POWER_REGULATOR                (0x2A)
#define GLCD_CMD_POWER_BOOSTER                  (0x2C)
#define GLCD_CMD_POWER_ALL                      (0x2F)
#define GLCD_CMD_ELECTRONIC_CONTROL(voltage)    (0x80 | (voltage))

/* GLCD Status bits */
#define GLCD_STATUS_BUSY                        (0x80)
#define GLCD_STATUS_ADC                         (0x40)
#define GLCD_STATUS_ON_OFF                      (0x20)
#define GLCD_STATUS_RESET                       (0x10)

/* Colors */
#define GLCD_COLOR_CLEAR                        (0x00)
#define GLCD_COLOR_SET                          (0x01)
#define GLCD_COLOR_INVERT                       (0x02)
#define GLCD_COLOR_COLLISION                    (0x03)		//used for collision detect, no actual drawing
#define GLCD_COLOR_FILLED                       (0x04)	

/* Indicators */
// #define GLCD_INDICATOR_0                        (20)
// #define GLCD_INDICATOR_1                        (31)
// #define GLCD_INDICATOR_2                        (32)
// #define GLCD_INDICATOR_3                        (57)
// #define GLCD_INDICATOR_4                        (69)
// #define GLCD_INDICATOR_5                        (78)
#define GLCD_MARKER_ARROWS_CROSS                   (20)
#define GLCD_MARKER_BARCODE_CROSS                  (31)
#define GLCD_MARKER_BARCODE                        (32)
#define GLCD_MARKER_BATTERY                        (57)
#define GLCD_MARKER_STAR                           (69)
#define GLCD_MARKER_ARROW_UP                       (78)

#define GLCD_IO_DELAY()
// #define GLCD_IO_DELAY()            __asm volatile ("nop")
// #define GLCD_IO_DELAY()            delayMicroseconds(1)
// #define GLCD_IO_DELAY_READ()            __asm volatile ("nop")
// #define GLCD_IO_DELAY_READ()            delayMicroseconds(1)
// #define GLCD_IO_DELAY_WRITE()           __asm volatile ("nop")
// #define GLCD_IO_DELAY_WRITE()           delayMicroseconds(1)

// #define GRAYOLED_SETCONTRAST 0x81   ///< Generic contrast for almost all OLEDs
// #define GRAYOLED_NORMALDISPLAY 0xA6 ///< Generic non-invert for almost all OLEDs
// #define GRAYOLED_INVERTDISPLAY 0xA7 ///< Generic invert for almost all OLEDs

// #define MONOOLED_BLACK 0   ///< Default black 'color' for monochrome OLEDS
// #define MONOOLED_WHITE 1   ///< Default white 'color' for monochrome OLEDS
// #define MONOOLED_INVERSE 2 ///< Default inversion command for monochrome OLEDS

typedef struct IO_CONFIG {
  uint8_t control_port;
  uint8_t control_pins;
  uint8_t data_port;
  uint8_t data_pins;
} io_config;

/*!
    @brief  Class that stores state and functions for interacting with
            generic grayscale OLED displays.
 */

class SED1530_LCD : public GFXcanvas1 {
public:
  #if !defined(IO_EXPANDER)
    SED1530_LCD(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA);
  #elif defined(IO_EXPANDER)
    // SED1530_LCD(uint8_t io_address);
    SED1530_LCD(RoboxIoExpander* io_ref);
  #endif

  ~SED1530_LCD(void);

  /**
   @brief The function that sub-classes define that writes out the buffer to
   the display over I2C or SPI
  **/
  // virtual void display(void) = 0;
  // void clearDisplay(void);
  // void setContrast(uint8_t contrastlevel);
  // void drawPixel(int16_t x, int16_t y, uint16_t color);
  // bool getPixel(int16_t x, int16_t y);
  void resetDisplay();
  void invertDisplay(bool i);
  void clearScreen(uint16_t color);
  void fillScreen(uint16_t color);
  // void drawPixel(int16_t x, int16_t y, uint16_t color);
  // void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  // void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void setMarker(uint8_t marker, bool on);
  void resetColumnAdress(void);
  void setContrast(uint8_t contrast);

  void writeCommand(uint8_t cmd);
  void writeData(uint8_t lcdData);

  // uint8_t index_buffer(uint8_t page, uint8_t column, uint8_t bit);
  void updateWholeScreen(void);

  // void oled_command(uint8_t c);
  // bool oled_commandList(const uint8_t *c, uint8_t n);

  void lcd_init(void);
protected:

  // Adafruit_SPIDevice *spi_dev = NULL; ///< The SPI interface BusIO device
  // Adafruit_I2CDevice *i2c_dev = NULL; ///< The I2C interface BusIO device
  // int32_t i2c_preclk = 400000,        ///< Configurable 'high speed' I2C rate
  //     i2c_postclk = 100000;           ///< Configurable 'low speed' I2C rate
  uint8_t *buffer = NULL; ///< Internal 1:1 framebuffer of display mem

  // int16_t window_x1, ///< Dirty tracking window minimum x
  //     window_y1,     ///< Dirty tracking window minimum y
  //     window_x2,     ///< Dirty tracking window maximum x
  //     window_y2;     ///< Dirty tracking window maximum y

  // int dcPin,  ///< The Arduino pin connected to D/C (for SPI)
  //     csPin,  ///< The Arduino pin connected to CS (for SPI)
  //     rstPin; ///< The Arduino pin connected to reset (-1 if unused)

  // uint8_t _bpp = 1; ///< Bits per pixel color for this display
private:
  #if !defined(IO_EXPANDER)
    uint8_t A0;
    uint8_t RW;
    uint8_t EN;
    uint8_t *DATA;
  #elif defined(IO_EXPANDER)
    io_config ports;
    RoboxIoExpander* io;
  #endif

  void setPage(uint8_t page);
  void setColumn(uint8_t row);

  // TwoWire *_theWire = NULL; ///< The underlying hardware I2C
};

#endif // end __AVR_ATtiny85__
#endif // _SED1530_LCD_H_