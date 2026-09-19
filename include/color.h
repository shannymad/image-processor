#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <stdexcept>

class Color {
public:
  Color() : r_(0), g_(0), b_(0) {}
  Color(double r, double g, double b) : r_(r), g_(g), b_(b) {}

  static Color FromBytes(uint8_t r, uint8_t g, uint8_t b) {
    return Color(r / 255.0, g / 255.0, b / 255.0);
  }

  double R() const { return r_; }
  double G() const { return g_; }
  double B() const { return b_; }

  double operator[](size_t i) const {
    switch (i) {
    case 0:
      return r_;
    case 1:
      return g_;
    case 2:
      return b_;
    default:
      throw std::out_of_range("Color index out of range");
    }
  }

  void Clamp() {
    r_ = std::clamp(r_, 0.0, 1.0);
    g_ = std::clamp(g_, 0.0, 1.0);
    b_ = std::clamp(b_, 0.0, 1.0);
  }

  Color Clamped() const {
    return Color(std::clamp(r_, 0.0, 1.0), std::clamp(g_, 0.0, 1.0),
                 std::clamp(b_, 0.0, 1.0));
  }

  Color operator+(const Color &rhs) const {
    return Color(r_ + rhs.r_, g_ + rhs.g_, b_ + rhs.b_);
  }

  Color operator-(const Color &rhs) const {
    return Color(r_ - rhs.r_, g_ - rhs.g_, b_ - rhs.b_);
  }

  Color operator*(double scalar) const {
    return Color(r_ * scalar, g_ * scalar, b_ * scalar);
  }

  Color &operator+=(const Color &rhs) {
    r_ += rhs.r_;
    g_ += rhs.g_;
    b_ += rhs.b_;
    return *this;
  }

  Color &operator-=(const Color &rhs) {
    r_ -= rhs.r_;
    g_ -= rhs.g_;
    b_ -= rhs.b_;
    return *this;
  }

  Color &operator*=(double scalar) {
    r_ *= scalar;
    g_ *= scalar;
    b_ *= scalar;
    return *this;
  }

  bool operator==(const Color &rhs) const {
    return std::abs(r_ - rhs.r_) < kEpsilon &&
           std::abs(g_ - rhs.g_) < kEpsilon && std::abs(b_ - rhs.b_) < kEpsilon;
  }

  bool operator!=(const Color &rhs) const { return !(*this == rhs); }

  std::array<uint8_t, 3> ToBytes() const {
    Color safe = Clamped();
    std::array<uint8_t, 3> bytes{};
    bytes[kR] = static_cast<uint8_t>(safe[kR] * 255);
    bytes[kG] = static_cast<uint8_t>(safe[kG] * 255);
    bytes[kB] = static_cast<uint8_t>(safe[kB] * 255);
    return bytes;
  }

private:
  double r_, g_, b_;

  static constexpr size_t kR = 0;
  static constexpr size_t kG = 1;
  static constexpr size_t kB = 2;
  static constexpr double kEpsilon = 1e-9;
};

inline Color operator*(double scalar, const Color &c) { return c * scalar; }

inline std::ostream &operator<<(std::ostream &os, const Color &c) {
  os << "Color(" << c.R() << ", " << c.G() << ", " << c.B() << ")";
  return os;
}