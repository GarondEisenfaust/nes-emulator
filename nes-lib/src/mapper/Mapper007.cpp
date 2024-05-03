#include "mapper/Mapper007.h"
#include "Definitions.h"

Mapper007::Mapper007(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode)
    : IMapper(programBanks, characterBanks, mirrorMode) {}

MappingResult Mapper007::CpuMapRead(uint16_t address) {
  if (0x8000 <= address && address <= 0xFFFF) {
    return {mBankSelect * 0x8000 + (address & 0x7FFF), {}};
  }
  return {};
}

MappingResult Mapper007::CpuMapWrite(uint16_t address, uint8_t data) {
  if (0x8000 <= address && address <= 0xFFFF) {
    mBankSelect = data & 0x07;
    mMirrorMode = data & 0x10 ? OneScreenLow : OneScreenHigh;
  }
  return {};
}

MappingResult Mapper007::PpuMapRead(uint16_t address) {
  if (0x0000 <= address && address <= 0x1FFF) {
    return {address & 0x1FFF, {}};
  }
  return {};
}

MappingResult Mapper007::PpuMapWrite(uint16_t address) {
  if (0x0000 <= address && address <= 0x1FFF) {
    return {address & 0x1FFF, {}};
  }
  return {};
}

void Mapper007::Reset() { mBankSelect = 0; }
