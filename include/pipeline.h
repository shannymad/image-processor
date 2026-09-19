#pragma once

#include <memory>
#include <vector>

#include "filter.h"
#include "image.h"

class Pipeline {
public:
  void AddFilter(std::unique_ptr<Filter> f);
  void Apply(Image &image);
  static Pipeline FromArgs(int argc, char **argv);

private:
  std::vector<std::unique_ptr<Filter>> filters_;
};