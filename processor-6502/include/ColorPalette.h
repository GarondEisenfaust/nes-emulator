#pragma once

#include "PixelColor.h"
#include <array>
#include <memory>

using ColorPalette = std::array<PixelColor, 56>;

std::unique_ptr<std::array<PixelColor, 56>> MakePixelColors();
