#pragma once
#include "MirrorMode.h"
#include "mapper/IMapper.h"
#include <cstdint>
#include <memory>
#include <vector>

#define CPU_CARTRIDGE_START 0x8000
#define CPU_CARTRIDGE_END 0xFFFF

#define PPU_CARTRIDGE_START 0x0000
#define PPU_CARTRIDGE_END 0x1FFF

class Cartridge {
 public:
  struct ReadResult {
    uint8_t readResult;
    bool hasRead;
  };

  Cartridge(const std::string& path);
  virtual ~Cartridge() = default;

  uint8_t CpuRead(uint16_t address);
  void CpuWrite(uint16_t address, uint8_t data);
  uint8_t PpuRead(uint16_t address);
  void PpuWrite(uint16_t address, uint8_t data);

  void Reset();
  MirrorMode mMirror;

 private:
  std::vector<uint8_t> mProgramMemory;
  std::vector<uint8_t> mCharacterMemory;

  uint8_t mMapperId;
  uint8_t mProgramBanks;
  uint8_t mCharacterBanks;
  bool mImageValid;

  std::unique_ptr<IMapper> mMapper;
};
