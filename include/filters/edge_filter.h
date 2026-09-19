#pragma once

#include "matrix_filter.h"

class Edge : public MatrixFilter {
public:
  explicit Edge(double threshold);
  Color PostProcess(const Color &c) const override;

private:
  double threshold_;
};