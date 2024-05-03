#pragma once
#include "IMapper.h"
#include <array>

class Mapper004 : public IMapper {
 public:
  Mapper004(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode);
  virtual ~Mapper004() = default;
  virtual MappingResult CpuMapRead(uint16_t address) override;
  virtual MappingResult CpuMapWrite(uint16_t address, uint8_t data) override;
  virtual MappingResult PpuMapRead(uint16_t address) override;
  virtual MappingResult PpuMapWrite(uint16_t address) override;
  virtual void Reset() override;
  virtual bool Interrupt() override;
  virtual void ClearInterrupt() override;
  virtual void ScanlineCounter() override;

 private:
  uint8_t mTargetRegister = 0x00;
  bool mProgramBankMode = false;
  bool mCharacterInversion = false;

  std::array<uint32_t, 8> mRegister;
  std::array<uint32_t, 8> mCharacterBanks;
  std::array<uint32_t, 4> mProgramBanks;

  bool mInteruptActive = false;
  bool mInteruptEnable = false;
  uint16_t mInteruptCounter = 0x0000;
  uint16_t mInteruptReloadValue = 0x0000;
  std::array<uint8_t, 32 * 1024> mMapperRam;

  constexpr const static int CHARACTER_BANK_SIZE = 0x0400;
  constexpr const static int PROGRAM_BANK_SIZE = 0x2000;

  void DetermineMirrorMode(bool evenAddress, uint8_t data);
  void ReloadInterruptCounter(bool evenAddress, uint8_t data);
  void UpdateBanks(bool evenAddress, uint8_t data);
  void DetermineInterruptEnable(bool evenAddress);
};
