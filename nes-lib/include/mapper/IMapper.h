#pragma once
#include "MirrorMode.h"
#include <cstdint>
#include <iostream>
#include <optional>

#define MAPPER_START 0x8000
#define MAPPER_END 0xFFFF

struct MappingResult {
  std::optional<uint32_t> mappedAddress;
  std::optional<uint8_t> data;
};

class IMapper {
 public:
  IMapper(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode)
      : mProgramBanks(programBanks), mCharacterBanks(characterBanks), mMirrorMode(mirrorMode) {}
  virtual ~IMapper() = default;

  virtual MappingResult CpuMapRead(uint16_t address) = 0;
  virtual MappingResult CpuMapWrite(uint16_t address, uint8_t data) = 0;
  virtual MappingResult PpuMapRead(uint16_t address) = 0;
  virtual MappingResult PpuMapWrite(uint16_t address) = 0;

  virtual void Reset() = 0;

  virtual bool Interrupt() { return false; }
  virtual void ClearInterrupt() {}
  virtual MirrorMode GetMirrorMode() { return mMirrorMode; }

 protected:
  uint8_t mProgramBanks;
  uint8_t mCharacterBanks;
  MirrorMode mMirrorMode;
};
