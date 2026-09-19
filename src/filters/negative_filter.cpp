#include "filters/negative_filter.h"

void Negative::Apply(Image &image) {
  for (Color &c : image.Pixels()) {
    c = Color(1 - c.R(), 1 - c.G(), 1 - c.B());
  }
}