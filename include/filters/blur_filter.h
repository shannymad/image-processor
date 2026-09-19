#pragma once

#include <vector>

#include "matrix_filter.h"

class Blur : public MatrixFilter {
public:
  explicit Blur(double sigma);

private:
  static std::vector<std::vector<double>> MakeGaussianKernel(double sigma);
};