#include "bmp_io.h"
#include "image.h"
#include <catch2/catch_all.hpp>
#include <cmath>
#include <cstdio>
#include <fstream>

TEST_CASE("BMP roundtrip simple") {
  Image original(10, 5, Color(1.0, 0.5, 0.0));
  original.SetPixel(3, 2, Color(0.0, 1.0, 0.0));
  original.SetPixel(9, 4, Color(0.0, 0.0, 1.0));

  WriteBMP(original, "/tmp/test.bmp");
  Image loaded = ReadBMP("/tmp/test.bmp");

  REQUIRE(loaded.Width() == 10);
  REQUIRE(loaded.Height() == 5);

  for (size_t y = 0; y < 5; ++y) {
    for (size_t x = 0; x < 10; ++x) {
      Color a = original.GetPixel(x, y);
      Color b = loaded.GetPixel(x, y);
      REQUIRE(std::abs(a.R() - b.R()) < 1e-2);
      REQUIRE(std::abs(a.G() - b.G()) < 1e-2);
      REQUIRE(std::abs(a.B() - b.B()) < 1e-2);
    }
  }

  std::remove("/tmp/test.bmp");
}

TEST_CASE("BMP with padding (width not multiple of 4)") {
  Image original(3, 3, Color(1, 0, 0));
  WriteBMP(original, "/tmp/pad.bmp");
  Image loaded = ReadBMP("/tmp/pad.bmp");

  REQUIRE(loaded.Width() == 3);
  REQUIRE(loaded.Height() == 3);
  Color c = loaded.GetPixel(1, 1);
  REQUIRE(std::abs(c.R() - 1.0) < 1e-2);
  REQUIRE(std::abs(c.G() - 0.0) < 1e-2);

  std::remove("/tmp/pad.bmp");
}

TEST_CASE("ReadBMP on non-BMP file throws") {
  std::ofstream out("/tmp/not_bmp.bmp", std::ios::binary);
  out << "this is not a BMP file";
  out.close();

  REQUIRE_THROWS_AS(ReadBMP("/tmp/not_bmp.bmp"), std::runtime_error);

  std::remove("/tmp/not_bmp.bmp");
}

TEST_CASE("ReadBMP on missing file throws") {
  REQUIRE_THROWS_AS(ReadBMP("/tmp/this_file_does_not_exist_12345.bmp"),
                    std::runtime_error);
}