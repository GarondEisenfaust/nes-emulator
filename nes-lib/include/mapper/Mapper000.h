#pragma once
#include "IMapper.h"

class Mapper000 : public IMapper {
 public:
  Mapper000() = default;
  Mapper000(uint8_t programBanks, uint8_t characterBanks);
  virtual ~Mapper000() = default;

  virtual bool CpuMapRead(uint16_t address, uint32_t& mappedAddr);
  virtual bool CpuMapWrite(uint16_t address, uint32_t& mappedAddr, uint8_t data);

  virtual bool PpuMapRead(uint16_t address, uint32_t& mappedAddr);
  virtual bool PpuMapWrite(uint16_t address, uint32_t& mappedAddr);

  virtual void Reset();

 private:
  uint8_t mProgramBanks;
  uint8_t mCharacterBanks;
};
