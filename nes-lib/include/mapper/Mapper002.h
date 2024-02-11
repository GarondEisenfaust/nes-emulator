#pragma once
#include "IMapper.h"

class Mapper002 : public IMapper {
 public:
  Mapper002(uint8_t programBanks, uint8_t characterBanks);
  virtual ~Mapper002() = default;

  virtual uint32_t CpuMapRead(uint16_t address);
  virtual uint32_t CpuMapWrite(uint16_t address, uint8_t data);
  virtual uint32_t PpuMapRead(uint16_t address);
  virtual uint32_t PpuMapWrite(uint16_t address);

  virtual void Reset();

 private:
  uint8_t mLowerSelectedProgramBank=0;
  uint8_t mUpperSelectedProgramBank=0;
};
