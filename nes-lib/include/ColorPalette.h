#pragma once

#include "PixelColor.h"
#include <array>
#include <memory>

using ColorPalette = std::array<PixelColor, 0x40>;

std::unique_ptr<ColorPalette> MakePixelColors();
