#pragma once

#include <cstddef>
#include <vector>

#include "filter.h"

class MatrixFilter : public Filter {
public:
  explicit MatrixFilter(std::vector<std::vector<double>> kernel,
                        double bias = 0);
  void Apply(Image &image) override;

protected:
  virtual Color PostProcess(const Color &c) const;

private:
  std::vector<std::vector<double>> kernel_;
  int radius_;
  double bias_;
  static size_t ClampCoord(int value, size_t max);
};
