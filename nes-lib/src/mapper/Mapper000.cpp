#include "mapper/Mapper000.h"
#include "Definitions.h"

Mapper000::Mapper000(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode)
    : IMapper(programBanks, characterBanks, mirrorMode) {}

MappingResult Mapper000::CpuMapRead(uint16_t address) { return {address & (mProgramBanks > 1 ? 0x7FFF : 0x3FFF)}; }
MappingResult Mapper000::CpuMapWrite(uint16_t address, uint8_t data) { return {CpuMapRead(address)}; }
MappingResult Mapper000::PpuMapRead(uint16_t address) { return {address}; }
MappingResult Mapper000::PpuMapWrite(uint16_t address) { return {address}; }

void Mapper000::Reset() {}
