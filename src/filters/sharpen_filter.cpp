#include "filters/sharpen_filter.h"

Sharpen::Sharpen() : MatrixFilter({{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}}) {}