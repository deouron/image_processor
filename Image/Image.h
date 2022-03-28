#ifndef CPP_PILOT_HSE_IMAGE_H
#define CPP_PILOT_HSE_IMAGE_H

#include "Color.h"

#include <cstdint>
#include <cstddef>
#include <vector>

class Image {
public:
    Image() = default;
    Image(size_t height, size_t width);
    Image(const std::vector<std::vector<Color>>& data);
    Image(const Image& other);
    ~Image() = default;

    size_t GetHeight() const;
    size_t GetWidth() const;
    const std::vector<std::vector<Color>>& GetData() const;
    Color& GetColor(size_t x, size_t y);
    const Color& GetColor(size_t x, size_t y) const;

    void SetColor(size_t x, size_t y, Color color);
private:
    void CheckHeightAndWidth(size_t x, size_t y) const;
    std::vector<std::vector<Color>> data_;
    size_t height_ = 0;
    size_t width_ = 0;
};

#endif  // CPP_PILOT_HSE_IMAGE_H
