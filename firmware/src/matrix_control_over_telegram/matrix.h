#pragma once

#include <Arduino.h>
#include <SPI.h>

const uint8_t SCAN_LIMIT_REG    = 0x0B;
const uint8_t SCAN_LIMIT_0_TO_7 = 0x07;

const uint8_t DECODE_MODE_REG   = 0x09;
const uint8_t NO_DECODE         = 0x00;

const uint8_t DISPLAY_TEST_REG  = 0x0F;
const uint8_t NORMAL_OPERATION  = 0x00;

class matrix {
private:
  uint8_t _width, _height;
  
  bool **_buffer;
  uint8_t _cs_pin;
  
  void _send_matrix_data(const uint8_t &ADDRESS, const uint8_t &DATA);

public:
  matrix(const uint8_t &CS_PIN, const uint8_t &WIDTH, const uint8_t &HEIGHT);

  ~matrix();

  void enable_matrix(const bool &ENABLE);
  
  void set_intensity(const uint8_t &INTENSITY);
  
  void set_led(const uint8_t &X, const uint8_t &Y, const bool &VALUE);
  
  void update_matrix();

  void clear_matrix();

  void erase_matrix();
};
