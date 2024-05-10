#ifndef _SERIAL_KEY_H
#define _SERIAL_KEY_H

// https://github.com/Spirik/GEM/issues/57

#include <Arduino.h>
// #include <GEM_u8g2.h>
#include "GEM_adafruit_gfx.h"

class SerialKey {
 public:
  SerialKey(int up_ascii_, int right_ascii_, int down_ascii_, int left_ascii_,
            int cancel_ascii_, int ok_ascii_);
  byte trigger = GEM_KEY_NONE;
  byte current = GEM_KEY_NONE;
  void detect();

 private:
  int _up_ascii;
  int _right_ascii;
  int _down_ascii;
  int _left_ascii;
  int _cancel_ascii;
  int _ok_ascii;
};

#endif