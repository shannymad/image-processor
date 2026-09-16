#include "bmp_io.h"

#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace {

uint16_t Read16(std::ifstream &in) {
  char c0 = in.get();
  char c1 = in.get();
  uint8_t b0 = static_cast<uint8_t>(c0);
  uint8_t b1 = static_cast<uint8_t>(c1);
  return static_cast<uint16_t>(b0) | (static_cast<uint16_t>(b1) << 8);
}

uint32_t Read32(std::ifstream &in) {
  char c0 = in.get();
  char c1 = in.get();
  char c2 = in.get();
  char c3 = in.get();
  uint8_t b0 = static_cast<uint8_t>(c0);
  uint8_t b1 = static_cast<uint8_t>(c1);
  uint8_t b2 = static_cast<uint8_t>(c2);
  uint8_t b3 = static_cast<uint8_t>(c3);
  return static_cast<uint32_t>(b0) | (static_cast<uint32_t>(b1) << 8) |
         (static_cast<uint32_t>(b2) << 16) | (static_cast<uint32_t>(b3) << 24);
}

void Write16(std::ofstream &out, uint16_t value) {
  out.put(static_cast<char>(value & 0xFF));
  out.put(static_cast<char>((value >> 8) & 0xFF));
}

void Write32(std::ofstream &out, uint32_t value) {
  out.put(static_cast<char>(value & 0xFF));
  out.put(static_cast<char>((value >> 8) & 0xFF));
  out.put(static_cast<char>((value >> 16) & 0xFF));
  out.put(static_cast<char>((value >> 24) & 0xFF));
}

} // namespace

Image ReadBMP(const std::string &path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    throw std::runtime_error("cannot open file: " + path);
  }

  char sig1 = in.get();
  char sig2 = in.get();
  if (sig1 != 'B' || sig2 != 'M') {
    throw std::runtime_error("not a BMP file: " + path);
  }

  (void)Read32(in);
  (void)Read16(in);
  (void)Read16(in);
  uint32_t data_offset = Read32(in);

  uint32_t header_size = Read32(in);
  uint32_t width = Read32(in);
  uint32_t height = Read32(in);
  (void)Read16(in);
  uint16_t bits_per_pixel = Read16(in);
  uint32_t compression = Read32(in);
  (void)Read32(in);
  (void)Read32(in);
  (void)Read32(in);
  (void)Read32(in);
  (void)Read32(in);

  if (header_size != 40) {
    throw std::runtime_error("unsupported BMP header size");
  }
  if (bits_per_pixel != 24) {
    throw std::runtime_error("unsupported BMP: only 24-bit supported");
  }
  if (compression != 0) {
    throw std::runtime_error("unsupported BMP: compression not supported");
  }

  in.seekg(data_offset);

  Image image(width, height);

  size_t padding = (4 - (width * 3) % 4) % 4;

  for (size_t file_row = 0; file_row < height; ++file_row) {
    size_t y = height - 1 - file_row;
    for (size_t x = 0; x < width; ++x) {
      char cb = in.get();
      char cg = in.get();
      char cr = in.get();
      uint8_t b = static_cast<uint8_t>(cb);
      uint8_t g = static_cast<uint8_t>(cg);
      uint8_t r = static_cast<uint8_t>(cr);
      image.SetPixel(x, y, Color::FromBytes(r, g, b));
    }
    for (size_t i = 0; i < padding; ++i) {
      in.get();
    }
  }

  return image;
}

void WriteBMP(const Image &image, const std::string &path) {
  std::ofstream out(path, std::ios::binary);
  if (!out) {
    throw std::runtime_error("cannot open file for writing: " + path);
  }

  uint32_t width = static_cast<uint32_t>(image.Width());
  uint32_t height = static_cast<uint32_t>(image.Height());
  uint32_t padding = (4 - (width * 3) % 4) % 4;
  uint32_t row_size = width * 3 + padding;
  uint32_t pixel_data_size = row_size * height;
  uint32_t file_size = 14 + 40 + pixel_data_size;
  uint32_t data_offset = 54;

  out.put('B');
  out.put('M');
  Write32(out, file_size);
  Write16(out, 0);
  Write16(out, 0);
  Write32(out, data_offset);

  Write32(out, 40);
  Write32(out, width);
  Write32(out, height);
  Write16(out, 1);
  Write16(out, 24);
  Write32(out, 0);
  Write32(out, pixel_data_size);
  Write32(out, 2835);
  Write32(out, 2835);
  Write32(out, 0);
  Write32(out, 0);

  for (size_t file_row = 0; file_row < height; ++file_row) {
    size_t y = height - 1 - file_row;
    for (size_t x = 0; x < width; ++x) {
      Color c = image.GetPixel(x, y);
      std::array<uint8_t, 3> rgb = c.ToBytes();
      out.put(static_cast<char>(rgb[2]));
      out.put(static_cast<char>(rgb[1]));
      out.put(static_cast<char>(rgb[0]));
    }
    for (size_t i = 0; i < padding; ++i) {
      out.put(0);
    }
  }
}