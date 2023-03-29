#pragma once
#include "MirrorMode.h"
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

  Cartridge(const std::string& path);
  virtual ~Cartridge() = default;

  bool CpuRead(uint16_t address, uint8_t& data);
  bool CpuWrite(uint16_t address, uint8_t data);

  bool PpuRead(uint16_t address, uint8_t& data);
  bool PpuWrite(uint16_t address, uint8_t data);
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
