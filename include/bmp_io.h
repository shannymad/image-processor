#pragma once

#include <string>

#include "image.h"

Image ReadBMP(const std::string& path);
void WriteBMP(const Image& image, const std::string& path);