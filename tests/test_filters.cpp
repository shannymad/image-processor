#include <catch2/catch_all.hpp>

#include <cmath>

#include "image.h"

#include "filters/blur_filter.h"
#include "filters/crop_filter.h"
#include "filters/crystallize_filter.h"
#include "filters/edge_filter.h"
#include "filters/glass_distortion_filter.h"
#include "filters/grayscale_filter.h"
#include "filters/negative_filter.h"
#include "filters/sharpen_filter.h"

TEST_CASE("Negative inverts colors") {
  Image img(2, 2, Color(1.0, 0.5, 0.0));
  Negative f;
  f.Apply(img);
  Color c = img.GetPixel(0, 0);
  REQUIRE(std::abs(c.R() - 0.0) < 1e-9);
  REQUIRE(std::abs(c.G() - 0.5) < 1e-9);
  REQUIRE(std::abs(c.B() - 1.0) < 1e-9);
}

TEST_CASE("Negative twice equals original") {
  Image img(3, 3, Color(0.3, 0.5, 0.7));
  Negative f;
  f.Apply(img);
  f.Apply(img);
  Color c = img.GetPixel(1, 1);
  REQUIRE(std::abs(c.R() - 0.3) < 1e-9);
  REQUIRE(std::abs(c.G() - 0.5) < 1e-9);
  REQUIRE(std::abs(c.B() - 0.7) < 1e-9);
}

TEST_CASE("GrayScale on red") {
  Image img(1, 1, Color(1.0, 0.0, 0.0));
  GrayScale f;
  f.Apply(img);
  Color c = img.GetPixel(0, 0);
  REQUIRE(std::abs(c.R() - 0.299) < 1e-9);
  REQUIRE(std::abs(c.G() - 0.299) < 1e-9);
  REQUIRE(std::abs(c.B() - 0.299) < 1e-9);
}

TEST_CASE("GrayScale on white stays white") {
  Image img(1, 1, Color(1, 1, 1));
  GrayScale f;
  f.Apply(img);
  REQUIRE(std::abs(img.GetPixel(0, 0).R() - 1.0) < 1e-9);
}

TEST_CASE("GrayScale makes all channels equal") {
  Image img(3, 3, Color(0.3, 0.7, 0.5));
  GrayScale f;
  f.Apply(img);
  for (size_t y = 0; y < 3; ++y) {
    for (size_t x = 0; x < 3; ++x) {
      Color c = img.GetPixel(x, y);
      REQUIRE(std::abs(c.R() - c.G()) < 1e-9);
      REQUIRE(std::abs(c.G() - c.B()) < 1e-9);
    }
  }
}

TEST_CASE("Crop crops to smaller size") {
  Image img(10, 10, Color(0.5, 0.5, 0.5));
  Crop f(5, 3);
  f.Apply(img);
  REQUIRE(img.Width() == 5);
  REQUIRE(img.Height() == 3);
}

TEST_CASE("Crop clamps to image size") {
  Image img(3, 3);
  Crop f(100, 100);
  f.Apply(img);
  REQUIRE(img.Width() == 3);
  REQUIRE(img.Height() == 3);
}

TEST_CASE("Crop takes top-left corner") {
  Image img(5, 5, Color(0, 0, 0));
  img.SetPixel(0, 0, Color(1, 1, 1));
  img.SetPixel(4, 4, Color(0.5, 0.5, 0.5));
  Crop f(3, 3);
  f.Apply(img);
  REQUIRE(std::abs(img.GetPixel(0, 0).R() - 1.0) < 1e-9);
  REQUIRE(std::abs(img.GetPixel(2, 2).R() - 0.0) < 1e-9);
}

TEST_CASE("Sharpen doesn't change flat image") {
  Image img(3, 3, Color(0.5, 0.5, 0.5));
  Sharpen f;
  f.Apply(img);
  for (size_t y = 0; y < 3; ++y) {
    for (size_t x = 0; x < 3; ++x) {
      REQUIRE(std::abs(img.GetPixel(x, y).R() - 0.5) < 1e-6);
    }
  }
}

TEST_CASE("Sharpen doesn't change size") {
  Image img(5, 3);
  Sharpen f;
  f.Apply(img);
  REQUIRE(img.Width() == 5);
  REQUIRE(img.Height() == 3);
}

TEST_CASE("Sharpen amplifies center") {
  Image img(3, 3, Color(0.2, 0.2, 0.2));
  img.SetPixel(1, 1, Color(0.8, 0.8, 0.8));
  Sharpen f;
  f.Apply(img);
  REQUIRE(img.GetPixel(1, 1).R() > 0.8);
}

TEST_CASE("Edge on flat image produces black") {
  Image img(5, 5, Color(0.5, 0.5, 0.5));
  Edge f(0.1);
  f.Apply(img);
  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 5; ++x) {
      REQUIRE(img.GetPixel(x, y).R() == 0.0);
    }
  }
}

TEST_CASE("Edge produces only black or white") {
  Image img(5, 5, Color(0.5, 0.5, 0.5));
  img.SetPixel(2, 2, Color(1, 1, 1));
  Edge f(0.1);
  f.Apply(img);
  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 5; ++x) {
      double v = img.GetPixel(x, y).R();
      REQUIRE((v == 0.0 || v == 1.0));
    }
  }
}

TEST_CASE("Edge with huge threshold produces black") {
  Image img(5, 5);
  img.SetPixel(2, 2, Color(1, 1, 1));
  Edge f(100.0);
  f.Apply(img);
  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 5; ++x) {
      REQUIRE(img.GetPixel(x, y).R() == 0.0);
    }
  }
}

TEST_CASE("Blur doesn't change flat image") {
  Image img(5, 5, Color(0.5, 0.5, 0.5));
  Blur f(1.0);
  f.Apply(img);
  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 5; ++x) {
      REQUIRE(std::abs(img.GetPixel(x, y).R() - 0.5) < 1e-6);
    }
  }
}

TEST_CASE("Blur doesn't change size") {
  Image img(7, 5);
  Blur f(1.0);
  f.Apply(img);
  REQUIRE(img.Width() == 7);
  REQUIRE(img.Height() == 5);
}

TEST_CASE("Blur smooths sharp edge") {
  Image img(5, 5, Color(0, 0, 0));
  for (size_t y = 0; y < 5; ++y) {
    img.SetPixel(4, y, Color(1, 1, 1));
  }
  Blur f(1.0);
  f.Apply(img);
  double left_of_edge = img.GetPixel(3, 2).R();
  REQUIRE(left_of_edge > 0.0);
}

TEST_CASE("Crystallize doesn't change size") {
  Image img(10, 10);
  Crystallize f(3.0);
  f.Apply(img);
  REQUIRE(img.Width() == 10);
  REQUIRE(img.Height() == 10);
}

TEST_CASE("Crystallize produces uniform blocks") {
  Image img(20, 20, Color(0.5, 0.5, 0.5));
  img.SetPixel(10, 10, Color(1, 0, 0));
  Crystallize f(5.0);
  f.Apply(img);
  size_t red_count = 0;
  for (size_t y = 0; y < 20; ++y) {
    for (size_t x = 0; x < 20; ++x) {
      if (img.GetPixel(x, y).R() > 0.9) {
        ++red_count;
      }
    }
  }
  REQUIRE(red_count > 1);
}

TEST_CASE("GlassDistortion with zero amplitude does nothing") {
  Image img(5, 5);
  img.SetPixel(2, 2, Color(1, 0, 0));
  Image original = img;
  GlassDistortion f(0.0, 10.0);
  f.Apply(img);
  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 5; ++x) {
      REQUIRE(std::abs(img.GetPixel(x, y).R() - original.GetPixel(x, y).R()) <
              1e-9);
    }
  }
}

TEST_CASE("GlassDistortion doesn't change size") {
  Image img(5, 5);
  GlassDistortion f(2.0, 10.0);
  f.Apply(img);
  REQUIRE(img.Width() == 5);
  REQUIRE(img.Height() == 5);
}
