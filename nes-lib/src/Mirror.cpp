#include "Mirror.h"
#include <stdexcept>

uint8_t& MirrorSingleScreen(std::array<std::array<uint8_t, 1024>, 2>& nameTable, uint16_t addr, MirrorMode mirror) {
  auto truncated = addr & 0x0FFF;
  return nameTable[mirror][truncated & 0x03FF];
}

uint8_t& MirrorHorizontal(std::array<std::array<uint8_t, 1024>, 2>& nameTable, uint16_t addr) {
  const auto truncated = addr & 0x0FFF;
  bool nameTableIndex;
  if (0x0000 <= truncated && truncated <= 0x03FF) {
    nameTableIndex = 0;
  } else if (0x0400 <= truncated && truncated <= 0x07FF) {
    nameTableIndex = 0;
  } else if (0x0800 <= truncated && truncated <= 0x0BFF) {
    nameTableIndex = 1;
  } else if (0x0C00 <= truncated && truncated <= 0x0FFF) {
    nameTableIndex = 1;
  } else {
    throw std::runtime_error("Address is not in the nametable address range!");
  }
  return nameTable[nameTableIndex][truncated & 0x03FF];
}

uint8_t& MirrorVertical(std::array<std::array<uint8_t, 1024>, 2>& nameTable, uint16_t addr) {
  const auto truncated = addr & 0x0FFF;
  bool nameTableIndex;
  if (0x0000 <= truncated && truncated <= 0x03FF) {
    nameTableIndex = 0;
  } else if (0x0400 <= truncated && truncated <= 0x07FF) {
    nameTableIndex = 1;
  } else if (0x0800 <= truncated && truncated <= 0x0BFF) {
    nameTableIndex = 0;
  } else if (0x0C00 <= truncated && truncated <= 0x0FFF) {
    nameTableIndex = 1;
  } else {
    throw std::runtime_error("Address is not in the nametable address range!");
  }
  return nameTable[nameTableIndex][truncated & 0x03FF];
}

uint8_t& Mirror(std::array<std::array<uint8_t, 1024>, 2>& nameTable, MirrorMode mirror, uint16_t addr) {
  if ((mirror == MirrorMode::OneScreenLow) || (mirror == MirrorMode::OneScreenHigh)) {
    return MirrorSingleScreen(nameTable, addr, mirror);
  } else if (mirror == MirrorMode::Vertical) {
    return MirrorVertical(nameTable, addr);
  } else if (mirror == MirrorMode::Horizontal) {
    return MirrorHorizontal(nameTable, addr);
  }
  throw std::runtime_error("Address could not be mirrored!");
}
