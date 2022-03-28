#include "Color.h"

Color::Color(uint8_t blue, uint8_t green, uint8_t red) {
    this->blue = blue;
    this->green = green;
    this->red = red;
}

void Color::SetValues(uint8_t input_blue, uint8_t input_green, uint8_t input_red) {
    blue = input_blue;
    green = input_green;
    red = input_red;
}
