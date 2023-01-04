#pragma once
#include "IMapper.h"

class Mapper000 : public IMapper {
 public:
  Mapper000() = default;
  Mapper000(uint8_t programBanks, uint8_t characterBanks);
  virtual ~Mapper000() = default;

  virtual MappingResult CpuMapRead(uint16_t address) override;
  virtual MappingResult CpuMapWrite(uint16_t address) override;

  virtual MappingResult PpuMapRead(uint16_t address) override;
  virtual MappingResult PpuMapWrite(uint16_t address) override;

  virtual void Reset();

 private:
  uint8_t mProgramBanks;
  uint8_t mCharacterBanks;
};
