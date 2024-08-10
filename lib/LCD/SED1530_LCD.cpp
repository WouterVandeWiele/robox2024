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

#include <mutex>
#include <stdint.h>
#include "Arduino.h"
#include "SED1530_LCD.h"
#include "Adafruit_GFX.h"
#include "robox_io.h"

// SED1530_LCD::SED1530_LCD(uint16_t w, uint16_t h, uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): GFXcanvas1(w, h), A0(A0), RW(RW), EN(EN), DATA(DATA) {

//     pinMode(this->A0, OUTPUT);
//     pinMode(this->RW, OUTPUT);
//     pinMode(this->EN, OUTPUT);

//     for (int i = 0; i <= 7; i++) {
//         pinMode(this->DATA[i], OUTPUT);
//     }

//     // digitalWrite(displayPower, LOW);
//     // delay(2000);
//     // digitalWrite(displayPower, HIGH);
//     // delay(3000);
//     this->init();
// }

#if !defined(IO_EXPANDER)
  SED1530_LCD::SED1530_LCD(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): GFXcanvas1(100, 48), A0(A0), RW(RW), EN(EN), DATA(DATA) {
      pinMode(this->A0, OUTPUT);
      pinMode(this->RW, OUTPUT);
      pinMode(this->EN, OUTPUT);

      for (int i = 0; i <= 7; i++) {
          pinMode(this->DATA[i], OUTPUT);
      }

      this->init();
  }
#elif defined(IO_EXPANDER)
  // SED1530_LCD::SED1530_LCD(uint8_t io_address): GFXcanvas1(100, 48), io(io_address), ports(ports) {

  // }
  SED1530_LCD::SED1530_LCD(RoboxIoExpander* io_ref): GFXcanvas1(100, 48), io(io_ref), ports(ports) {

    // ESP_LOGI(LOG_LCD_TAG, "lcd io setup start");
    // io->configure_outputs(LCD_CONTROL_PORT, ~(LCD_A0 | LCD_RW | LCD_EN));
    // io->configure_outputs(LCD_DATA_PORT, ~LCD_DATA_PINS);
    // ESP_LOGI(LOG_LCD_TAG, "lcd io setup done");
    // // this->init();
    // ESP_LOGI(LOG_LCD_TAG, "lcd init done");
  }
#endif

SED1530_LCD::~SED1530_LCD(void) {
  if (buffer) {
    free(buffer);
    buffer = NULL;
  }
}

void SED1530_LCD::writeCommand(uint8_t cmd) {
  // ESP_LOGI(LOG_LCD_TAG, "lcd write command %2x", cmd);
  #if !defined(IO_EXPANDER)
    digitalWrite(this->RW, LOW);
    digitalWrite(this->A0, LOW);
    
    byte data = cmd;
    
    for (int i = 7; i >= 0; i--) {
        int value = data & 0x1;
        digitalWrite(this->DATA[i], value);
        data = data >> 1;
    }

    digitalWrite(this->EN, HIGH);
    GLCD_IO_DELAY();
    digitalWrite(this->EN, LOW);
    GLCD_IO_DELAY();
    digitalWrite(this->EN, HIGH);
  #elif defined(IO_EXPANDER)
    io->set_output(LCD_CONTROL_PORT, LCD_EN, (LCD_A0 | LCD_RW | LCD_EN));
    io->set_output(LCD_DATA_PORT, cmd, LCD_DATA_PINS);

    io->set_output(LCD_CONTROL_PORT, 0, (LCD_A0 | LCD_RW | LCD_EN));
    io->set_output(LCD_CONTROL_PORT, LCD_EN, (LCD_A0 | LCD_RW | LCD_EN));
  #endif
}

void SED1530_LCD::writeData(uint8_t lcdData) {
  // ESP_LOGI(LOG_LCD_TAG, "lcd write data %2x", lcdData);
  #if !defined(IO_EXPANDER)
    digitalWrite(this->RW, LOW);
    digitalWrite(this->A0, HIGH);

    byte data = lcdData;

    for (int i = 7; i >= 0 ; i--) {
        int value = data & 0x1;
        digitalWrite(this->DATA[i], value);
        data = data >> 1;
    }

    digitalWrite(this->EN, HIGH);
    GLCD_IO_DELAY();
    digitalWrite(this->EN, LOW);
    GLCD_IO_DELAY();
    digitalWrite(this->EN, HIGH);
  #elif defined(IO_EXPANDER)
    io->set_output(LCD_CONTROL_PORT, (LCD_A0 | LCD_EN), (LCD_A0 | LCD_RW | LCD_EN));
    io->set_output(LCD_DATA_PORT, lcdData, LCD_DATA_PINS);

    io->set_output(LCD_CONTROL_PORT, LCD_A0, (LCD_A0 | LCD_RW | LCD_EN));
    io->set_output(LCD_CONTROL_PORT, (LCD_A0 | LCD_EN), (LCD_A0 | LCD_RW | LCD_EN));

  #endif
}

void SED1530_LCD::lcd_init() {
  std::lock_guard<std::mutex> lck(io_operations);
  // ESP_LOGI(LOG_LCD_TAG, "lcd io setup start");
  // io->set_output(LCD_DATA_PORT, 0x00);
  // io->set_output(LCD_CONTROL_PORT, 0x00);
  // io->configure_outputs(LCD_CONTROL_PORT, ~(LCD_EN | LCD_RW | LCD_A0));
  // io->configure_outputs(LCD_DATA_PORT, ~(LCD_DATA_PINS));
  // ESP_LOGI(LOG_LCD_TAG, "lcd io setup done");
  // this->init();
  
  //the following actions are performed to init the lcd
  // delay(100);
  this->writeCommand(0xe2);    //reset display by soft
  delay(1500);
  this->writeCommand(0xa1);    //ADC select
  this->writeCommand(0xa2);    //lcd bias 1/8
  this->writeCommand(0x2c);    //power
  this->writeCommand(0x2e);    //power
  this->writeCommand(0x2f);    //power
  this->writeCommand(0xa6);    //normal / reverse
  this->writeCommand(0x8f);    //set electronic control
  this->writeCommand(0xa4);    //display off
  this->writeCommand(0xaf);    //display on
  this->writeCommand(0x40);    //sel dram line 1 for com1

  ESP_LOGI(LOG_LCD_TAG, "lcd init done");

}

void SED1530_LCD::resetDisplay() {
  std::lock_guard<std::mutex> lck(io_operations);
  this->writeCommand(GLCD_CMD_RESET);
}

void SED1530_LCD::invertDisplay(bool i) {
  std::lock_guard<std::mutex> lck(io_operations);
  this->writeCommand(0xA6 + (i ? 0:1));
}

/*  Control de markers boven in het display
 GLCD_MARKER_ARROWS_CROSS   20  -- pijltjes boven elkaar
 GLCD_MARKER_BARCODE_CROSS  31  -- 2 horiz. streepjes van barcode
 GLCD_MARKER_BARCODE        32  -- barcode
 GLCD_MARKER_BATTERY        57  -- batterij
 GLCD_MARKER_STAR           69  -- sterretje
 GLCD_MARKER_ARROW_UP       78  -- pijltje UP
*/
void SED1530_LCD::setMarker(uint8_t marker, bool on) {
    std::lock_guard<std::mutex> lck(io_operations);
    uint8_t highNibble, lowNibble;
    uint8_t markerLCD;

    // switch (marker) {
    //     case 1 : markerLCD = 20;
    //             break;
    //     case 2 : markerLCD = 31;
    //             break;
    //     case 3: markerLCD = 32;
    //             break;
    //     case 4: markerLCD = 57;
    //             break;
    //     case 5: markerLCD = 69;
    //         break;
    //     case 6: markerLCD = 78;
    //         break;
    // }
    
    lowNibble = marker & 0xF;      // Mask out upper nibble
    highNibble = marker;        
    highNibble = highNibble >> 4;     // Shift upper 4 bits to lower
    bitSet(highNibble, 4);            // Set 5th bit high
    
    this->writeCommand(0xb6);        //Set page Address
    this->writeCommand(highNibble);  //Set column Address high nibble
    this->writeCommand(lowNibble);   //Set column Address low nibble
    
    this->writeData(on ? 1 : 0);
}

/*
 Reset column address to the first position in the page
*/
void SED1530_LCD::resetColumnAdress(void) {
  std::lock_guard<std::mutex> lck(io_operations);
  this->writeCommand(0x10);
  this->writeCommand(0x00);
}

/*
Constrast is a value between 0 and 31.
*/
void SED1530_LCD::setContrast(uint8_t contrast) {
  std::lock_guard<std::mutex> lck(io_operations);
  this->writeCommand(0x80 + contrast & 0x3F);
}

void SED1530_LCD::setPage(uint8_t page) {
  this->writeCommand(GLCD_CMD_SET_PAGE(page));
  this->writeCommand(0x08);
  this->writeCommand(0x00);
    
}
void SED1530_LCD::setColumn(uint8_t row) {
  this->writeCommand(GLCD_CMD_SET_COLUMN_HIGH(row >> 4));
  this->writeCommand(GLCD_CMD_SET_COLUMN_LOW(row & 0x0F));
}

// uint8_t SED1530_LCD::index_buffer(uint8_t page, uint8_t column, uint8_t bit) {
//   uint16_t jump_width = ((this->WIDTH + 7) / 8);  // # bytes is 1 column
//   uint16_t pages = this->HEIGHT / 8;              // # pages in the visible part of the display
  
//   return this->buffer[(page*jump_width) + (column * bit) + column];
// }

// void SED1530_LCD::drawPixel(int16_t x, int16_t y, uint16_t color) {

//   uint8_t page = y / 8;
//   uint8_t data = 0x00;

//   // must update the canvas object, bacause we use it to get the column byte values
//   this->GFXcanvas1::drawPixel(x, y, color);

//   // bits of the page are stored in different canvas bytes
//   for(uint8_t b = 0; b < 8; b++) {
//     data |= this->getPixel(x, ((page*8)+b)) << b;
//   }

//   // Serial.printf("(%d, %d) %02x\n", x, y, data);
//   this->setPage(page);
//   this->setColumn(x);
//   this->writeData(data);
// }


void SED1530_LCD::clearScreen(uint16_t color) {
  std::lock_guard<std::mutex> lck(io_operations);
  // draw all pages
  for (uint16_t p = 0; p < 8; p++) {
    this->setPage(p);
    this->setColumn(0);

    // draw each column in the page
    for (uint16_t c = 0; c < 132; c++) {
      this->writeData(color);
    }
  }
}

void SED1530_LCD::fillScreen(uint16_t color) {
  std::lock_guard<std::mutex> lck(io_operations);
  uint8_t pages = this->HEIGHT / 8;
  uint8_t data = color ? 0xFF : 0x00;

  // must update the canvas object, bacause we use it to get the column byte values
  this->GFXcanvas1::fillScreen(color);

  // draw all pages
  for (uint16_t p = 0; p < pages; p++) {
    this->setPage(p);
    this->setColumn(0);

    // draw each column in the page
    for (uint16_t c = 0; c < this->WIDTH; c++) {
      this->writeData(data);
    }
  }
}

void SED1530_LCD::updateWholeScreen(void) {
  std::lock_guard<std::mutex> lck(io_operations);
  // uint32_t bytes = ((w + 7) / 8) * h;
  // if ((buffer = (uint8_t *)malloc(bytes))) {
  //   memset(buffer, 0, bytes);
  // }
  // uint16_t height_page = 8;             // # bits in the page hight
  uint16_t jump_width = ((this->WIDTH + 7) / 8);  // # bytes is 1 column
  uint16_t pages = this->HEIGHT / 8;           // # pages in the visible part of the display

  // cast the canvas object to a 2D array
  // uint8_t (*canvas)[pages][jump_width] = (uint8_t(*)[pages][jump_width])this->buffer;

  // draw all pages
  for (uint16_t p = 0; p < pages; p++){
    this->setPage(p);
    this->setColumn(0);

    // draw each column in the page
    for (uint16_t c = 0; c < this->WIDTH; c++){

      // uint8_t mask = 1 << (c % 8);
      uint8_t data = 0x00;


      // bits of the page are stored in different canvas bytes
      for(uint8_t b = 0; b < 8; b++) {
        // uint8_t column_canvas_byte = *canvas[p+b][c/8];
        data |= this->getPixel(c, (p*8)+b) << b;
        // uint8_t column_canvas_byte = index_buffer(p, c, b);

        // data |= (column_canvas_byte & mask);
        // if ( column_canvas_byte & mask) {
        //   bitSet(data, b);
        // }
      }

      this->writeData(data);
      // vTaskDelay(1);
    }
    // vTaskDelay(1);
    // delay(1);
  }
}