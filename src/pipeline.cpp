#include "pipeline.h"

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "filters/blur_filter.h"
#include "filters/crop_filter.h"
#include "filters/crystallize_filter.h"
#include "filters/edge_filter.h"
#include "filters/glass_distortion_filter.h"
#include "filters/grayscale_filter.h"
#include "filters/negative_filter.h"
#include "filters/sharpen_filter.h"

namespace {

using FilterFactory =
    std::function<std::unique_ptr<Filter>(const std::vector<std::string> &)>;

std::unique_ptr<Filter> MakeNegative(const std::vector<std::string> &args) {
  if (!args.empty()) {
    throw std::runtime_error("-neg takes no arguments");
  }
  return std::make_unique<Negative>();
}

std::unique_ptr<Filter> MakeGrayscale(const std::vector<std::string> &args) {
  if (!args.empty()) {
    throw std::runtime_error("-gs takes no arguments");
  }
  return std::make_unique<GrayScale>();
}

std::unique_ptr<Filter> MakeSharpen(const std::vector<std::string> &args) {
  if (!args.empty()) {
    throw std::runtime_error("-sharp takes no arguments");
  }
  return std::make_unique<Sharpen>();
}

std::unique_ptr<Filter> MakeCrop(const std::vector<std::string> &args) {
  if (args.size() != 2) {
    throw std::runtime_error(
        "-crop requires exactly 2 arguments (width height)");
  }
  size_t width = std::stoul(args[0]);
  size_t height = std::stoul(args[1]);
  return std::make_unique<Crop>(width, height);
}

std::unique_ptr<Filter> MakeEdge(const std::vector<std::string> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("-edge requires exactly 1 argument (threshold)");
  }
  double threshold = std::stod(args[0]);
  return std::make_unique<Edge>(threshold);
}

std::unique_ptr<Filter> MakeBlur(const std::vector<std::string> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("-blur requires exactly 1 argument (sigma)");
  }
  double sigma = std::stod(args[0]);
  return std::make_unique<Blur>(sigma);
}

std::unique_ptr<Filter> MakeCrystallize(const std::vector<std::string> &args) {
  if (args.size() != 1) {
    throw std::runtime_error(
        "-crystallize requires exactly 1 argument (radius)");
  }
  double radius = std::stod(args[0]);
  return std::make_unique<Crystallize>(radius);
}

std::unique_ptr<Filter>
MakeGlassDistortion(const std::vector<std::string> &args) {
  if (args.size() != 2) {
    throw std::runtime_error(
        "-glass requires exactly 2 arguments (amplitude period)");
  }
  double amplitude = std::stod(args[0]);
  double period = std::stod(args[1]);
  return std::make_unique<GlassDistortion>(amplitude, period);
}

struct FilterInfo {
  int arg_count;
  FilterFactory factory;
};

const std::unordered_map<std::string, FilterInfo> kFilters = {
    {"-neg", {0, MakeNegative}},
    {"-gs", {0, MakeGrayscale}},
    {"-crop", {2, MakeCrop}},
    {"-sharp", {0, MakeSharpen}},
    {"-edge", {1, MakeEdge}},
    {"-blur", {1, MakeBlur}},
    {"-crystallize", {1, MakeCrystallize}},
    {"-glass", {2, MakeGlassDistortion}},
};

} // namespace

void Pipeline::AddFilter(std::unique_ptr<Filter> f) {
  filters_.push_back(std::move(f));
}

void Pipeline::Apply(Image &image) {
  for (auto &f : filters_) {
    f->Apply(image);
  }
}

Pipeline Pipeline::FromArgs(int argc, char **argv) {
  Pipeline p;
  int i = 3;

  while (i < argc) {
    std::string name = argv[i];

    auto it = kFilters.find(name);
    if (it == kFilters.end()) {
      throw std::runtime_error("unknown filter: " + name);
    }

    const FilterInfo &info = it->second;
    int required = info.arg_count;

    if (i + required >= argc) {
      throw std::runtime_error("filter " + name + " requires " +
                               std::to_string(required) + " argument(s)");
    }

    std::vector<std::string> args;
    for (int k = 1; k <= required; ++k) {
      args.push_back(argv[i + k]);
    }

    auto filter = info.factory(args);
    p.AddFilter(std::move(filter));

    i += 1 + required;
  }

  return p;
}
