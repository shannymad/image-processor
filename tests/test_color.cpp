#include <catch2/catch_all.hpp>

#include <cmath>

#include "color.h"

TEST_CASE("Color default is black") {
  Color c;
  REQUIRE(c.R() == 0.0);
  REQUIRE(c.G() == 0.0);
  REQUIRE(c.B() == 0.0);
}

TEST_CASE("Color from doubles") {
  Color c(0.1, 0.2, 0.3);
  REQUIRE(c.R() == 0.1);
  REQUIRE(c.G() == 0.2);
  REQUIRE(c.B() == 0.3);
}

TEST_CASE("FromBytes converts to [0, 1]") {
  Color c = Color::FromBytes(255, 128, 0);
  REQUIRE(c.R() == 1.0);
  REQUIRE(std::abs(c.G() - 0.50196) < 1e-4);
  REQUIRE(c.B() == 0.0);
}

TEST_CASE("Clamped clamps to [0, 1]") {
  Color c(1.3, -0.2, 0.5);
  Color safe = c.Clamped();
  REQUIRE(safe.R() == 1.0);
  REQUIRE(safe.G() == 0.0);
  REQUIRE(safe.B() == 0.5);
  REQUIRE(c.R() == 1.3);
}

TEST_CASE("operator[] reads correct channel") {
  Color c(0.1, 0.2, 0.3);
  REQUIRE(c[0] == 0.1);
  REQUIRE(c[1] == 0.2);
  REQUIRE(c[2] == 0.3);
  REQUIRE_THROWS_AS(c[5], std::out_of_range);
}

TEST_CASE("ToBytes converts to uint8_t") {
  Color c(0.0, 0.5, 1.0);
  auto bytes = c.ToBytes();
  REQUIRE(bytes[0] == 0);
  REQUIRE(bytes[1] == 127);
  REQUIRE(bytes[2] == 255);
}

TEST_CASE("ToBytes clamps out-of-range") {
  Color c(1.5, -0.5, 0.5);
  auto bytes = c.ToBytes();
  REQUIRE(bytes[0] == 255);
  REQUIRE(bytes[1] == 0);
  REQUIRE(bytes[2] == 127);
}

TEST_CASE("operator+ adds componentwise") {
  Color a(0.1, 0.2, 0.3);
  Color b(0.4, 0.5, 0.6);
  Color c = a + b;
  REQUIRE(std::abs(c.R() - 0.5) < 1e-9);
  REQUIRE(std::abs(c.G() - 0.7) < 1e-9);
  REQUIRE(std::abs(c.B() - 0.9) < 1e-9);
}

TEST_CASE("operator- subtracts componentwise") {
  Color a(0.5, 0.5, 0.5);
  Color b(0.1, 0.2, 0.3);
  Color c = a - b;
  REQUIRE(std::abs(c.R() - 0.4) < 1e-9);
  REQUIRE(std::abs(c.G() - 0.3) < 1e-9);
  REQUIRE(std::abs(c.B() - 0.2) < 1e-9);
}

TEST_CASE("operator* multiplies by scalar") {
  Color a(0.1, 0.2, 0.3);
  Color c = a * 2.0;
  REQUIRE(std::abs(c.R() - 0.2) < 1e-9);
  REQUIRE(std::abs(c.G() - 0.4) < 1e-9);
  REQUIRE(std::abs(c.B() - 0.6) < 1e-9);
}