#include "include/filters/sharpen_filter.h"

void Sharpen::Apply(Image &image) {
  Image copy = image;

  for (size_t y = 0; y < image.Height(); ++y) {
    for (size_t x = 0; x < image.Width(); ++x) {
      Color center = copy.GetPixel(x, y);
      size_t y_up = (y > 0) ? (y - 1) : 0;
      size_t y_down = (y + 1 < image.Height()) ? (y + 1) : (image.Height() - 1);
      size_t x_left = (x > 0) ? (x - 1) : 0;
      size_t x_right = (x + 1 < image.Width()) ? (x + 1) : (image.Width() - 1);

      Color up = copy.GetPixel(x, y_up);
      Color down = copy.GetPixel(x, y_down);
      Color left = copy.GetPixel(x_left, y);
      Color right = copy.GetPixel(x_right, y);

      double new_channels[3];

      for (size_t ch = 0; ch < 3; ++ch) {
        new_channels[ch] =
            5 * center[ch] - up[ch] - down[ch] - left[ch] - right[ch];
      }

      Color new_color(new_channels[0], new_channels[1], new_channels[2]);
      image.SetPixel(x, y, new_color);
    }
  }
}