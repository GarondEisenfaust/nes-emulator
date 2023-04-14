#pragma once

#include "PixelColor.h"
#include <array>
#include <memory>

#define NUMBER_OF_COLORS 64

using ColorPalette = std::array<PixelColor, NUMBER_OF_COLORS>;
std::unique_ptr<ColorPalette> MakePixelColors();
