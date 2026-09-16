#pragma once

#include "filter.h"

class Crop : public Filter {
public:
  Crop(size_t width, size_t height);
  void Apply(Image &image) override;

private:
  size_t width_, height_;
};
