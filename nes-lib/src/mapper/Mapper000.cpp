#include "mapper/Mapper000.h"
#include "Definitions.h"

Mapper000::Mapper000(uint8_t programBanks, uint8_t characterBanks)
    : mProgramBanks(programBanks), mCharacterBanks(characterBanks) {}

bool Mapper000::CpuMapRead(uint16_t address, uint32_t& mappedAddr) {
  if ((MAPPER_START <= address) && (address <= MAPPER_END)) {
    mappedAddr = address & (mProgramBanks > 1 ? 0x7FFF : 0x3FFF);
    return true;
  }
  return false;
}

bool Mapper000::CpuMapWrite(uint16_t address, uint32_t& mappedAddr, uint8_t data) {
  return CpuMapRead(address, mappedAddr);
}

bool Mapper000::PpuMapRead(uint16_t address, uint32_t& mappedAddr) {
  if (address >= 0x0000 && address <= 0x1FFF) {
    mappedAddr = address;
    return true;
  }

  return false;
}

bool Mapper000::PpuMapWrite(uint16_t address, uint32_t& mappedAddr) {
  if (address >= 0x0000 && address <= 0x1FFF) {
    if (mCharacterBanks == 0) {
      mappedAddr = address;
      return true;
    }
  }

  return false;
}

void Mapper000::Reset() {}
