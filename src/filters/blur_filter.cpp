#include "filters/blur_filter.h"

#include <cmath>
#include <stdexcept>

namespace {

constexpr double kPi = 3.14159265358979323846;

} // namespace

Blur::Blur(double sigma) : MatrixFilter(MakeGaussianKernel(sigma)) {}

std::vector<std::vector<double>> Blur::MakeGaussianKernel(double sigma) {
  if (sigma <= 0.0) {
    throw std::invalid_argument("Blur: sigma must be positive");
  }

  int radius = static_cast<int>(std::ceil(3.0 * sigma));
  int size = 2 * radius + 1;

  std::vector<std::vector<double>> kernel(size, std::vector<double>(size, 0.0));

  double two_sigma_sq = 2.0 * sigma * sigma;
  double norm = kPi * two_sigma_sq;

  for (int ky = 0; ky < size; ++ky) {
    for (int kx = 0; kx < size; ++kx) {
      int dx = kx - radius;
      int dy = ky - radius;
      double value = std::exp(-(dx * dx + dy * dy) / two_sigma_sq) / norm;
      kernel[ky][kx] = value;
    }
  }

  double sum = 0.0;
  for (int ky = 0; ky < size; ++ky) {
    for (int kx = 0; kx < size; ++kx) {
      sum += kernel[ky][kx];
    }
  }

  for (int ky = 0; ky < size; ++ky) {
    for (int kx = 0; kx < size; ++kx) {
      kernel[ky][kx] /= sum;
    }
  }

  return kernel;
}
