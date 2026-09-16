#include "include/filters/crop_filter.h"
#include <algorithm>

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {}

Crop::Apply(Image &image) {
  size_t actual_width = std::min(width_, image.Width());
  size_t actual_height = std::min(height_, image.Height());

  Image cropped(actual_width, actual_height);

  for (size_t y = 0; y < actual_height; ++y) {
    for (size_t x = 0; x < actual_width; ++x) {
      cropped.SetPixel(x, y, image.GetPixel(x, y));
    }
  }
  image = cropped;
}