#include "matrix.h"

#define CS_PIN        7
#define MATRIX_WIDTH  8
#define MATRIX_HEIGHT 8

matrix m(CS_PIN, MATRIX_WIDTH, MATRIX_HEIGHT);

void setup() {
  Serial.begin(9600);
  m.enable_matrix(1);
  m.set_intensity(1);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readString();

    if (data == "/clear") {
      m.erase_matrix();
      m.update_matrix();
      return;
    }
    
    String x_string, y_string, value_string;
    bool detected_x = false, detected_y = false, detected_value = false;
    
    for (uint8_t i = 0; i < data.length(); ++i) {
      if (data[i] == 'x') {
        detected_x = true;
        detected_y = false;
        detected_value = false;
      }
      else if (data[i] == 'y') {
        detected_x = false;
        detected_y = true;
        detected_value = false;
      }
      else if (data[i] == 's') {
        detected_x = false;
        detected_y = false;
        detected_value = true;
      }
      else {
        if (detected_x) x_string += data[i];
        if (detected_y) y_string += data[i];
        if (detected_value) value_string += data[i];
      }
    }

    uint8_t x = atoi(x_string.c_str());
    uint8_t y = atoi(y_string.c_str());
    uint8_t value = atoi(value_string.c_str());

    // Swap x and y values. Works fine, because matrix size is 8x8 LEDs.
    m.set_led(MATRIX_HEIGHT - y - 1, x, value == 0 ? 0 : 1);
    m.update_matrix();
  }
}
