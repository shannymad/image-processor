#include "filters/crystallize_filter.h"

#include <cmath>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <vector>

namespace {

struct Crystal {
  size_t x;
  size_t y;
  Color color;
};

} // namespace

Crystallize::Crystallize(double radius) : radius_(radius) {
  if (radius_ <= 0.0) {
    throw std::invalid_argument("Crystallize: radius must be positive");
  }
}

void Crystallize::Apply(Image &image) {
  Image copy = image;
  size_t width = image.Width();
  size_t height = image.Height();

  if (width == 0 || height == 0) {
    return;
  }

  std::mt19937 gen(42);
  std::uniform_real_distribution<double> dist(0.0, radius_);

  std::vector<Crystal> crystals;
  for (double cy = 0.0; cy < static_cast<double>(height); cy += radius_) {
    for (double cx = 0.0; cx < static_cast<double>(width); cx += radius_) {
      double jitter_x = dist(gen);
      double jitter_y = dist(gen);
      size_t px = static_cast<size_t>(
          std::min(cx + jitter_x, static_cast<double>(width - 1)));
      size_t py = static_cast<size_t>(
          std::min(cy + jitter_y, static_cast<double>(height - 1)));
      crystals.push_back({px, py, copy.GetPixel(px, py)});
    }
  }

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      double best_dist = 1e18;
      Color best_color = copy.GetPixel(x, y);
      for (const auto &c : crystals) {
        double dx = static_cast<double>(x) - static_cast<double>(c.x);
        double dy = static_cast<double>(y) - static_cast<double>(c.y);
        double d = dx * dx + dy * dy;
        if (d < best_dist) {
          best_dist = d;
          best_color = c.color;
        }
      }
      image.SetPixel(x, y, best_color);
    }
  }
}