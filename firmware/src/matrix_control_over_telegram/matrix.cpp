#include "matrix.h"

void matrix::_send_matrix_data(const uint8_t &ADDRESS, const uint8_t &DATA) {
  digitalWrite(_cs_pin, 0);
  SPI.transfer(ADDRESS);
  SPI.transfer(DATA);
  digitalWrite(_cs_pin, 1);
}

matrix::matrix(const uint8_t &CS_PIN, const uint8_t &WIDTH, const uint8_t &HEIGHT) {
  _cs_pin = CS_PIN;
  pinMode(CS_PIN, OUTPUT);
  
  _width = WIDTH;
  _height = HEIGHT;
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  
  enable_matrix(0);
  _send_matrix_data(SCAN_LIMIT_REG, SCAN_LIMIT_0_TO_7);
  _send_matrix_data(DECODE_MODE_REG, NO_DECODE);
  set_intensity(8);
  _send_matrix_data(DISPLAY_TEST_REG, NORMAL_OPERATION);
  
  clear_matrix();
  
  _buffer = (bool**)malloc(HEIGHT * sizeof(bool*));
  for (uint8_t y = 0; y < HEIGHT; ++y) {
    _buffer[y] = (bool*)malloc(WIDTH * sizeof(bool));
  }

  erase_matrix();
}

matrix::~matrix() {
  free(_buffer);
}

void matrix::enable_matrix(const bool &ENABLE) {
  _send_matrix_data(0x0C, ENABLE ? 0x01 : 0x00);
}

void matrix::set_intensity(const uint8_t &INTENSITY) {
  _send_matrix_data(0x0A, INTENSITY > 15 ? 15 : INTENSITY);
}

void matrix::set_led(const uint8_t &X, const uint8_t &Y, const bool &VALUE) {
  _buffer[Y][X] = VALUE;
}

void matrix::update_matrix() {
  for (uint8_t y = 0; y < _height; ++y) {
    uint8_t column_data = 0;
    for (uint8_t x = 0; x < _width; ++x) {
      if (_buffer[y][x]) column_data |= (1 << (_width - 1 - x));
    }
    _send_matrix_data(y + 1, column_data);
  }
}

void matrix::clear_matrix() {
  for (int y = 1; y <= _height; ++y) {
    _send_matrix_data(y, 0);
  }
}

void matrix::erase_matrix() {
  for (uint8_t y = 0; y < _height; ++y) {
    for (uint8_t x = 0; x < _width; ++x) {
      _buffer[y][x] = 0;
    }
  }
}
