#pragma once

#include "filter.h"

class GlassDistortion : public Filter {
public:
  GlassDistortion(double amplitude, double period);
  void Apply(Image &image) override;

private:
  double amplitude_;
  double period_;
};