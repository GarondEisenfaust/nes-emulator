#pragma once
#include <array>
#include <cstdint>

#define RAM_START 0x0000
#define RAM_END 0x1FFF
#define RAM_SIZE 0x07FF

#define VRAM_SIZE 0x0400
#define VRAM_NUMBER 2
#define PALETTE_SIZE 32

using RAM = std::array<uint8_t, RAM_SIZE + 1>;
using VRAM = std::array<uint8_t, VRAM_SIZE>;
using Palette = std::array<uint8_t, PALETTE_SIZE>;
