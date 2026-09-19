#include "filters/grayscale_filter.h"

void GrayScale::Apply(Image &image) {
  for (Color &c : image.Pixels()) {
    double gray = 0.299 * c.R() + 0.587 * c.G() + 0.114 * c.B();
    c = Color(gray, gray, gray);
  }
}