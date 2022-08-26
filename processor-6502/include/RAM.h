#pragma once
#include <array>
#include <cstdint>

const auto ram_size = 64 * 1024;
using RAM = std::array<uint8_t, ram_size>;
