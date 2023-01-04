#include "mapper/Mapper000.h"
#include "Definitions.h"

Mapper000::Mapper000(uint8_t programBanks, uint8_t characterBanks)
    : mProgramBanks(programBanks), mCharacterBanks(characterBanks) {}

MappingResult Mapper000::CpuMapRead(uint16_t address) {
  if ((MAPPER_START <= address) && (address <= MAPPER_END)) {
    uint32_t mappedAddr = address & (mProgramBanks > 1 ? 0x7FFF : 0x3FFF);
    return {mappedAddr, true};
  }
  return {UNDEFINED, false};
}

MappingResult Mapper000::CpuMapWrite(uint16_t address) { return CpuMapRead(address); }

MappingResult Mapper000::PpuMapRead(uint16_t address) {
  if (address >= 0x0000 && address <= 0x1FFF) {
    return {address, true};
  }

  return {UNDEFINED, false};
}

MappingResult Mapper000::PpuMapWrite(uint16_t address) {
  return mCharacterBanks == 0 ? PpuMapRead(address) : MappingResult{UNDEFINED, false};
}

void Mapper000::Reset() {}
