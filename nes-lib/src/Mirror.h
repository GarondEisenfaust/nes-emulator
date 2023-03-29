#pragma once
#include "MirrorMode.h"
#include <array>
#include <cstdint>

uint8_t& Mirror(std::array<std::array<uint8_t, 1024>, 2>& nameTable, MirrorMode mirror, uint16_t addr);
