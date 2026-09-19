#include "matrix_filter.h"

#include <stdexcept>
#include <utility>

MatrixFilter::MatrixFilter(std::vector<std::vector<double>> kernel, double bias)
    : kernel_(std::move(kernel)), radius_(0), bias_(bias) {
  if (kernel_.empty()) {
    throw std::invalid_argument("MatrixFilter: empty kernel");
  }

  size_t size = kernel_.size();

  if (size % 2 == 0) {
    throw std::invalid_argument("MatrixFilter: even kernel size");
  }

  for (const auto &row : kernel_) {
    if (row.size() != size) {
      throw std::invalid_argument("MatrixFilter: non-square kernel");
    }
  }

  radius_ = static_cast<int>(size / 2);
}

void MatrixFilter::Apply(Image &image) {
  Image copy = image;
  size_t width = image.Width();
  size_t height = image.Height();

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      double new_r = 0.0;
      double new_g = 0.0;
      double new_b = 0.0;

      for (int ky = 0; ky < 2 * radius_ + 1; ++ky) {
        for (int kx = 0; kx < 2 * radius_ + 1; ++kx) {
          int dx = kx - radius_;
          int dy = ky - radius_;
          Color neighbor = copy.GetPixelClamped(static_cast<int>(x) + dx,
                                                static_cast<int>(y) + dy);
          double coeff = kernel_[ky][kx];
          new_r += coeff * neighbor.R();
          new_g += coeff * neighbor.G();
          new_b += coeff * neighbor.B();
        }
      }

      new_r += bias_;
      new_g += bias_;
      new_b += bias_;

      Color result(new_r, new_g, new_b);
      result = PostProcess(result);
      image.SetPixel(x, y, result);
    }
  }
}

Color MatrixFilter::PostProcess(const Color &c) const { return c.Clamped(); }
