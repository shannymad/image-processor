#pragma once 

#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <array>

class Color {
    public: 

    Color() : r_(0), g_(0), b_(0) {};

    Color(double r, double g, double b) : r_(r), g_(g), b_(b){};

    static Color FromBytes(uint8_t r, uint8_t g, uint8_t b) {
        return Color(r / 255.0, g / 255.0, b / 255.0);
    }

    double operator[](size_t i) const {
        switch (i) {
            case 0: return r_;
            case 1:  return g_;
            case 2: return b_;
            default: throw std::out_of_range("...");
        }

    }

    double R() const {
        return r_;
    }

    double G() const {
        return g_;
    }

    double B() const {
        return b_;
    }
    void Clamp() {
        r_ = std::clamp(r_, 0.0, 1.0);
        g_ = std::clamp(g_, 0.0, 1.0);
        b_ = std::clamp(b_, 0.0, 1.0);
    }


    Color Clamped() const {
        return Color(std::clamp(r_, 0.0, 1.0), std::clamp(g_, 0.0, 1.0), std::clamp(b_, 0.0, 1.0));

    }

    Color operator+(const Color& rhs) const {
        return Color(r_ + rhs.r_, g_ + rhs.g_, b_ + rhs.b_);
    }

    Color operator*(double scalar) const {
        return Color(r_ * scalar, g_ * scalar, b_ * scalar);
    }

    std::array<uint8_t, 3> ToBytes() const {
        Color safe = Clamped();

        std::array<uint8_t, 3> bytes;

       bytes[R] = static_cast<uint8_t>(safe[R] * 255);
       bytes[G] = static_cast<uint8_t>(safe[G] * 255);
       bytes[B] = static_cast<uint8_t>(safe[B] * 255);
        return bytes;


    }



    private: 
    double r_, g_, b_;

    static constexpr size_t R = 0;
    static constexpr size_t G = 1;
    static constexpr size_t B = 2;

};