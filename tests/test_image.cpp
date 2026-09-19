#include <catch2/catch_all.hpp>

#include "image.h"

TEST_CASE("Image default is empty") {
  Image img;
  REQUIRE(img.Width() == 0);
  REQUIRE(img.Height() == 0);
}

TEST_CASE("Image with size is filled with black") {
  Image img(10, 5);
  REQUIRE(img.Width() == 10);
  REQUIRE(img.Height() == 5);
  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 10; ++x) {
      REQUIRE(img.GetPixel(x, y).R() == 0.0);
    }
  }
}

TEST_CASE("Image with fill color") {
  Image img(3, 3, Color(1.0, 0.5, 0.0));
  Color c = img.GetPixel(1, 1);
  REQUIRE(c.R() == 1.0);
  REQUIRE(c.G() == 0.5);
  REQUIRE(c.B() == 0.0);
}

TEST_CASE("Set and get pixel roundtrip") {
  Image img(5, 5);
  img.SetPixel(2, 3, Color(0.5, 0.5, 0.5));
  REQUIRE(img.GetPixel(2, 3).R() == 0.5);
  REQUIRE(img.GetPixel(0, 0).R() == 0.0);
}

TEST_CASE("GetPixel out of range throws") {
  Image img(5, 5);
  REQUIRE_THROWS_AS(img.GetPixel(5, 0), std::out_of_range);
  REQUIRE_THROWS_AS(img.GetPixel(0, 5), std::out_of_range);
}

TEST_CASE("SetPixel out of range throws") {
  Image img(5, 5);
  REQUIRE_THROWS_AS(img.SetPixel(5, 0, Color()), std::out_of_range);
}

TEST_CASE("Fill sets all pixels") {
  Image img(3, 3);
  img.Fill(Color(0.5, 0.5, 0.5));
  for (size_t y = 0; y < 3; ++y) {
    for (size_t x = 0; x < 3; ++x) {
      REQUIRE(img.GetPixel(x, y).R() == 0.5);
    }
  }
}