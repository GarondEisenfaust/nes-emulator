#pragma once
#include <array>
#include <cstdint>

#define RAM_START 0x0000
#define RAM_END 0x1FFF
#define RAM_SIZE 0x0800

using RAM = std::array<uint8_t, RAM_SIZE>;
