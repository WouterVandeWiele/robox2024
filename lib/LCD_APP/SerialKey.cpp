#include "SerialKey.h"

#include <Arduino.h>
// #include <GEM_u8g2.h>
#include "GEM_adafruit_gfx.h"

// https://github.com/Spirik/GEM/issues/57

SerialKey::SerialKey(int up_ascii_, int right_ascii_, int down_ascii_,
                     int left_ascii_, int cancel_ascii_, int ok_ascii_)
    : _up_ascii(up_ascii_),
      _right_ascii(right_ascii_),
      _down_ascii(down_ascii_),
      _left_ascii(left_ascii_),
      _cancel_ascii(cancel_ascii_),
      _ok_ascii(ok_ascii_) {}

void SerialKey::detect() {
  boolean pressed = false;

  if (Serial.available() > 0) {
    int incomingByte = Serial.read();

    // Serial.print("Received: ");
    // Serial.println(incomingByte, DEC);

    if (incomingByte == _up_ascii) {
      current = GEM_KEY_UP;
      pressed = true;
      // Serial.println("Pressed up");
    } else if (incomingByte == _right_ascii) {
      current = GEM_KEY_RIGHT;
      pressed = true;
      // Serial.println("Pressed right");
    } else if (incomingByte == _down_ascii) {
      current = GEM_KEY_DOWN;
      pressed = true;
      // Serial.println("Pressed down");
    } else if (incomingByte == _left_ascii) {
      current = GEM_KEY_LEFT;
      pressed = true;
      // Serial.println("Pressed left");
    } else if (incomingByte == _cancel_ascii) {
      current = GEM_KEY_CANCEL;
      pressed = true;
      // Serial.println("Pressed cancel");
    } else if (incomingByte == _ok_ascii) {
      current = GEM_KEY_OK;
      pressed = true;
      // Serial.println("Pressed ok");
    }
  } else {
    pressed = false;
  }

  if (!pressed) {
    current = GEM_KEY_NONE;
  }

  trigger = current;
}