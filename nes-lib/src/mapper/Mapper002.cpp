#include "mapper/Mapper002.h"

Mapper002::Mapper002(uint8_t programBanks, uint8_t characterBanks) : IMapper(programBanks, characterBanks) {}

uint32_t Mapper002::CpuMapRead(uint16_t address) {
  if (0x8000 <= address && address <= 0xBFFF) {
    return mLowerSelectedProgramBank * 0x4000 + (address & 0x3FFF);
  }

  if (0xC000 <= address && address <= 0xFFFF) {
    return mUpperSelectedProgramBank * 0x4000 + (address & 0x3FFF);
  }

  return address;
}

uint32_t Mapper002::CpuMapWrite(uint16_t address, uint8_t data) {
  mLowerSelectedProgramBank = data & 0x0F;
  return address;
}
uint32_t Mapper002::PpuMapRead(uint16_t address) { return address; }
uint32_t Mapper002::PpuMapWrite(uint16_t address) { return address; }

void Mapper002::Reset() {
  mLowerSelectedProgramBank = 0;
  mUpperSelectedProgramBank = mProgramBanks - 1;
}
