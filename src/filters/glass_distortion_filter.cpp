#include "filters/glass_distortion_filter.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace {

constexpr double kPi = 3.14159265358979323846;

size_t ClampCoord(int value, size_t max) {
  if (value < 0)
    return 0;
  if (static_cast<size_t>(value) >= max)
    return max - 1;
  return static_cast<size_t>(value);
}

} // namespace

GlassDistortion::GlassDistortion(double amplitude, double period)
    : amplitude_(amplitude), period_(period) {
  if (period_ <= 0.0) {
    throw std::invalid_argument("GlassDistortion: period must be positive");
  }
}

void GlassDistortion::Apply(Image &image) {
  Image copy = image;
  size_t width = image.Width();
  size_t height = image.Height();

  if (width == 0 || height == 0) {
    return;
  }

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      double dx =
          amplitude_ * std::sin(2.0 * kPi * static_cast<double>(y) / period_);
      double dy =
          amplitude_ * std::cos(2.0 * kPi * static_cast<double>(x) / period_);

      int nx = static_cast<int>(std::round(static_cast<double>(x) + dx));
      int ny = static_cast<int>(std::round(static_cast<double>(y) + dy));

      size_t ux = ClampCoord(nx, width);
      size_t uy = ClampCoord(ny, height);

      image.SetPixel(x, y, copy.GetPixel(ux, uy));
    }
  }
}