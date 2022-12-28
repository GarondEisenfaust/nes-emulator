#pragma once
#include "mapper/IMapper.h"
#include <cstdint>
#include <memory>
#include <vector>

class Cartridge {
 public:
  struct ReadResult {
    uint8_t readResult;
    bool hasRead;
  };

  enum MIRROR { HORIZONTAL, VERTICAL, ONESCREEN_LO, ONESCREEN_HI };

  Cartridge(const std::string& path);
  virtual ~Cartridge() = default;

  ReadResult CpuRead(uint16_t address);
  bool CpuWrite(uint16_t address, uint8_t data);

  ReadResult PpuRead(uint16_t address);
  bool PpuWrite(uint16_t address, uint8_t data);
  void Reset();
  MIRROR mMirror;

 private:
  std::vector<uint8_t> mProgramMemory;
  std::vector<uint8_t> mCharacterMemory;

  uint8_t mMapperId;
  uint8_t mProgramBanks;
  uint8_t mCharacterBanks;
  bool mImageValid;

  std::unique_ptr<IMapper> mMapper;
};
