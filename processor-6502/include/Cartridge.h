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

  enum MIRROR {
    HORIZONTAL,
    VERTICAL,
  };

  Cartridge(const std::string& path);
  virtual ~Cartridge() = default;

  ReadResult Read(uint16_t address);
  bool Write(uint16_t address, uint8_t data);

 private:
  std::vector<uint8_t> mProgramMemory;
  std::vector<uint8_t> mCharacterMemory;

  uint8_t mMapperId;
  uint8_t mProgramBanks;
  uint8_t mCharacterBanks;
  bool mImageValid;
  MIRROR mMirror;

  std::unique_ptr<IMapper> mMapper;
};
