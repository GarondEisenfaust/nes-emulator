#pragma once

#include "PixelColor.h"
#include <array>
#include <memory>

#define NUMBER_OF_COLORS 64

using ColorPaletteU8 = std::array<PixelColorU8, NUMBER_OF_COLORS>;
using ColorPaletteF = std::array<PixelColorF, NUMBER_OF_COLORS>;

std::unique_ptr<ColorPaletteU8> MakePixelColors();
std::unique_ptr<ColorPaletteF> MakePixelColorsFloat();
