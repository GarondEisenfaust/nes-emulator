#pragma once
#include "IMapper.h"

class Mapper007 : public IMapper {
 public:
  Mapper007(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode);
  virtual ~Mapper007() = default;

  virtual MappingResult CpuMapRead(uint16_t address);
  virtual MappingResult CpuMapWrite(uint16_t address, uint8_t data);
  virtual MappingResult PpuMapRead(uint16_t address);
  virtual MappingResult PpuMapWrite(uint16_t address);

  virtual void Reset();

 private:
  int mBankSelect = 0;
};
