#include "Mirror.h"
#include "fmt/format.h"
#include <stdexcept>

uint8_t& MirrorHorizontal(std::array<std::array<uint8_t, 1024>, 2>& nameTable, uint16_t addr) {
  auto truncated = addr & 0x0FFF;
  if (0x0000 <= truncated && truncated <= 0x03FF) {
    return nameTable[0][truncated & 0x03FF];
  }
  if (0x0400 <= truncated && truncated <= 0x07FF) {
    return nameTable[0][truncated & 0x03FF];
  }
  if (0x0800 <= truncated && truncated <= 0x0BFF) {
    return nameTable[1][truncated & 0x03FF];
  }
  if (0x0C00 <= truncated && truncated <= 0x0FFF) {
    return nameTable[1][truncated & 0x03FF];
  }
  throw std::runtime_error(fmt::format("The address {:#06x} is not in the nametable address range!", addr));
}

uint8_t& MirrorVertical(std::array<std::array<uint8_t, 1024>, 2>& nameTable, uint16_t addr) {
  auto truncated = addr & 0x0FFF;
  if (0x0000 <= truncated && truncated <= 0x03FF) {
    return nameTable[0][truncated & 0x03FF];
  }
  if (0x0400 <= truncated && truncated <= 0x07FF) {
    return nameTable[1][truncated & 0x03FF];
  }
  if (0x0800 <= truncated && truncated <= 0x0BFF) {
    return nameTable[0][truncated & 0x03FF];
  }
  if (0x0C00 <= truncated && truncated <= 0x0FFF) {
    return nameTable[1][truncated & 0x03FF];
  }
  throw std::runtime_error(fmt::format("The address {:#06x} is not in the nametable address range!", addr));
}

uint8_t& Mirror(std::array<std::array<uint8_t, 1024>, 2>& nameTable, MirrorMode mirror, uint16_t addr) {
  if (mirror == MirrorMode::Vertical) {
    return MirrorVertical(nameTable, addr);
  } else if (mirror == MirrorMode::Horizontal) {
    return MirrorHorizontal(nameTable, addr);
  }
  throw std::runtime_error(fmt::format("The address {:#06x} could not be mirrored!", addr));
}
