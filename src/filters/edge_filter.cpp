#include "filters/edge_filter.h"

Edge::Edge(double threshold)
    : MatrixFilter({{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}),
      threshold_(threshold) {}

Color Edge::PostProcess(const Color &c) const {
  if (c.R() > threshold_) {
    return Color(1, 1, 1);
  }
  return Color(0, 0, 0);
}