#pragma once

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "color.h"

class Image {
public:
  Image() : width_(0), height_(0) {}

  Image(size_t width, size_t height) : Image(width, height, Color()) {}

  Image(size_t width, size_t height, Color fill)
      : width_(width), height_(height), pixels_(width * height, fill) {}

  size_t Width() const { return width_; }
  size_t Height() const { return height_; }

  Color GetPixel(size_t x, size_t y) const {
    CheckBounds(x, y);
    return pixels_[y * width_ + x];
  }

  Color GetPixelClamped(int x, int y) const {
    if (width_ == 0 || height_ == 0) {
      throw std::out_of_range("GetPixelClamped on empty image");
    }
    int cx = std::clamp(x, 0, static_cast<int>(width_) - 1);
    int cy = std::clamp(y, 0, static_cast<int>(height_) - 1);
    return pixels_[static_cast<size_t>(cy) * width_ + static_cast<size_t>(cx)];
  }

  void SetPixel(size_t x, size_t y, Color color) {
    CheckBounds(x, y);
    pixels_[y * width_ + x] = color;
  }

  std::vector<Color> &Pixels() { return pixels_; }
  const std::vector<Color> &Pixels() const { return pixels_; }

  void Fill(Color color) { std::fill(pixels_.begin(), pixels_.end(), color); }

private:
  size_t width_, height_;
  std::vector<Color> pixels_;

  void CheckBounds(size_t x, size_t y) const {
    if (x >= width_ || y >= height_) {
      throw std::out_of_range("image coordinates out of range");
    }
  }
};
