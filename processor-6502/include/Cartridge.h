#pragma once
#include <cstdint>
#include <vector>

class Cartridge {
 public:
  Cartridge();
  ~Cartridge() = default;

  uint8_t Read(uint16_t addr);
  void Write(uint16_t addr, uint8_t data);

 private:
  std::vector<uint8_t> mProgramMemory;
  std::vector<uint8_t> mPCharacterMemory;

  uint8_t mMapperId;
  uint8_t ProgramBanks;
  uint8_t mCharacterBanks;
};
