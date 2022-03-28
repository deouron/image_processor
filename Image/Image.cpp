#include "Image.h"

#include <stdexcept>
#include <utility>

Image::Image(size_t height, size_t width) : data_(height, std::vector<Color>(width)), height_(height), width_(width) {
}

Image::Image(const std::vector<std::vector<Color>>& data) :
      data_(data),
      height_(data.size()),
      width_(data.front().size()) {
}

Image::Image(const Image& other) {
    height_ = other.height_;
    width_ = other.width_;
    data_ = other.GetData();
}

size_t Image::GetHeight() const {
    return height_;
}

size_t Image::GetWidth() const {
    return width_;
}

void Image::CheckHeightAndWidth(size_t x, size_t y) const {
    if (x > height_ || y > width_) {
        throw std::invalid_argument("Coordinates are larger than the image size during GetColor or SetColor");
    }
}

Color& Image::GetColor(size_t x, size_t y) {
    CheckHeightAndWidth(x, y);
    return data_[x][y];
}

const Color& Image::GetColor(size_t x, size_t y) const {
    CheckHeightAndWidth(x, y);
    return data_[x][y];
}

void Image::SetColor(size_t x, size_t y, Color color) {
    CheckHeightAndWidth(x, y);
    data_[x][y] = color;
}

const std::vector<std::vector<Color>>& Image::GetData() const {
    return data_;
}
