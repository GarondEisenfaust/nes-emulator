#pragma once
#include "IMapper.h"

class Mapper000 : public IMapper {
 public:
  Mapper000(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode);
  virtual ~Mapper000() = default;

  virtual MappingResult CpuMapRead(uint16_t address);
  virtual MappingResult CpuMapWrite(uint16_t address, uint8_t data);
  virtual MappingResult PpuMapRead(uint16_t address);
  virtual MappingResult PpuMapWrite(uint16_t address);

  virtual void Reset();
};
