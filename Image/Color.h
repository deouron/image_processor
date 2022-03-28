#pragma once

#include <cstdint>

struct Color {
    Color() = default;
    Color(uint8_t blue, uint8_t green, uint8_t red);
    uint8_t red = 0;  // 0 - 255
    uint8_t green = 0;
    uint8_t blue = 0;

    void SetValues(uint8_t input_blue, uint8_t input_green, uint8_t input_red);
};
