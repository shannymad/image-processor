#pragma once

#include "filter.h"

class GrayScale : public Filter {
public:
  void Apply(Image &image) override;
  ~GrayScale() override = default;
};