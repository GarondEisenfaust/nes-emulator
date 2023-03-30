#include "mapper/Mapper000.h"
#include "Definitions.h"

Mapper000::Mapper000(uint8_t programBanks, uint8_t characterBanks)
    : mProgramBanks(programBanks), mCharacterBanks(characterBanks) {}

uint32_t Mapper000::CpuMapRead(uint16_t address) { return address & (mProgramBanks > 1 ? 0x7FFF : 0x3FFF); }
uint32_t Mapper000::CpuMapWrite(uint16_t address) { return CpuMapRead(address); }
uint32_t Mapper000::PpuMapRead(uint16_t address) { return address; }
uint32_t Mapper000::PpuMapWrite(uint16_t address) { return address; }

void Mapper000::Reset() {}
