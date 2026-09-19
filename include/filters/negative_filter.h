#pragma once
#include "filter.h"

class Negative : public Filter {
public:
  void Apply(Image &image) override;
  ~Negative() override = default;
};
