#pragma once

#include "filter.h"

class Crystallize : public Filter {
public:
  explicit Crystallize(double radius);
  void Apply(Image &image) override;

private:
  double radius_;
};