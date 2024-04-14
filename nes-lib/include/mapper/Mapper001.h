#pragma once
#include "IMapper.h"
#include <MirrorMode.h>
#include <array>
#include <cstdint>

class Mapper001 : public IMapper {
 public:
  Mapper001(uint8_t programBankCount, uint8_t characterBankCount, MirrorMode mirrorMode);
  virtual ~Mapper001() = default;

  virtual MappingResult CpuMapRead(uint16_t address);
  virtual MappingResult CpuMapWrite(uint16_t address, uint8_t data);
  virtual MappingResult PpuMapRead(uint16_t address);
  virtual MappingResult PpuMapWrite(uint16_t address);

  void Reset();

 private:
  uint8_t mCharacterBankSelectLower = 0x00;
  uint8_t mCharacterBankSelectHigher = 0x00;
  uint8_t mCharacterBankSelect = 0x00;

  uint8_t mProgramBankSelectLower = 0x00;
  uint8_t mProgramBankSelectHigher = 0x00;
  uint8_t mProgramBankSelect = 0x00;

  class LoadRegister {
   public:
    void Reset();
    void PushIntoLoadRegister(uint8_t data);
    bool Full();
    uint8_t operator&(const uint8_t byte);

   private:
    uint8_t mRegister = 0x00;
    uint8_t mCount = 0x00;
  };
  LoadRegister mLoadRegister;
  uint8_t mControlRegister = 0x00;

  std::array<uint8_t, 32 * 1024> mMapperRam;

  void HandleFullLoadRegister(uint16_t address);
  void TargetRegisterZero();
  void TargetRegisterOne();
  void TargetRegisterTwo();
  void TargetRegisterThree();
};
