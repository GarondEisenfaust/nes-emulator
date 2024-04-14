#include "mapper/Mapper004.h"

Mapper004::Mapper004(uint8_t programBanks, uint8_t characterBanks, MirrorMode mirrorMode)
    : IMapper(programBanks, characterBanks, mirrorMode) {}

MappingResult Mapper004::CpuMapRead(uint16_t address) {
  if (0x6000 <= address && address <= 0x7FFF) {
    return {{}, mMapperRam[address & 0x1FFF]};
  }

  if (0x8000 <= address && address <= 0xFFFF) {
    int bank = (address - 0x8000) / PROGRAM_BANK_SIZE;
    return {{mProgramBanks[bank] + (address & 0x1FFF)}, {}};
  }
  return {{address}, {}};
}

MappingResult Mapper004::CpuMapWrite(uint16_t address, uint8_t data) {
  const bool isEvenAddress = address % 2 == 0;
  if (0x6000 <= address && address <= 0x7FFF) {
    mMapperRam[address & 0x1FFF] = data;
  } else if (0x8000 <= address && address <= 0x9FFF) {
    UpdateBanks(isEvenAddress, data);
  } else if (0xA000 <= address && address <= 0xBFFF) {
    DetermineMirrorMode(isEvenAddress, data);
  } else if (0xC000 <= address && address <= 0xDFFF) {
    ReloadInterruptCounter(isEvenAddress, data);
  } else if (0xE000 <= address && address <= 0xFFFF) {
    DetermineInterruptEnable(isEvenAddress);
  }

  return {};
}

MappingResult Mapper004::PpuMapRead(uint16_t address) {
  if (0x0000 <= address && address <= 0x1FFF) {
    int bank = address / CHARACTER_BANK_SIZE;
    return {mCharacterBanks[bank] + (address & 0x03FF), {}};
  }

  return {{address}, {}};
}

MappingResult Mapper004::PpuMapWrite(uint16_t address) { return {}; }

void Mapper004::Reset() {
  mTargetRegister = 0x00;
  mProgramBankMode = false;
  mCharacterInversion = false;
  mMirrorMode = Horizontal;

  mInteruptActive = false;
  mInteruptEnable = false;
  mInteruptReload = false;
  mInteruptCounter = 0;
  mInteruptReloadValue = 0;

  mProgramBanks = {0};
  mCharacterBanks = {0};
  mRegister = {0};

  mProgramBanks[0] = 0 * PROGRAM_BANK_SIZE;
  mProgramBanks[1] = 1 * PROGRAM_BANK_SIZE;
  mProgramBanks[2] = (mProgramBankCount * 2 - 2) * PROGRAM_BANK_SIZE;
  mProgramBanks[3] = (mProgramBankCount * 2 - 1) * PROGRAM_BANK_SIZE;
}

bool Mapper004::Interrupt() { return mInteruptActive; }

void Mapper004::ClearInterrupt() { mInteruptActive = false; }

void Mapper004::ScanlineCounter() {
  if (mInteruptReload) {
    mInteruptCounter = mInteruptReloadValue;
    mInteruptReload = false;
  } else if (mInteruptCounter == 0) {
    mInteruptCounter = mInteruptReloadValue;
  } else {
    mInteruptCounter--;
  }
  if (mInteruptCounter == 0 && mInteruptEnable) {
    mInteruptActive = true;
  }
}

void Mapper004::DetermineMirrorMode(bool evenAddress, uint8_t data) {
  if (evenAddress) {
    if (data & 0x01) {
      mMirrorMode = Horizontal;
    } else {
      mMirrorMode = Vertical;
    }
  }
}

void Mapper004::ReloadInterruptCounter(bool evenAddress, uint8_t data) {
  if (evenAddress) {
    mInteruptReloadValue = data;
  } else {
    mInteruptReload = true;
    mInteruptCounter = 0;
  }
}

void Mapper004::UpdateBanks(bool evenAddress, uint8_t data) {
  if (evenAddress) {
    mTargetRegister = data & 0x07;
    mProgramBankMode = data & 0x40;
    mCharacterInversion = data & 0x80;
  } else {
    mRegister[mTargetRegister] = data;
  }
  if (mCharacterInversion) {
    mCharacterBanks[0] = mRegister[2] * CHARACTER_BANK_SIZE;
    mCharacterBanks[1] = mRegister[3] * CHARACTER_BANK_SIZE;
    mCharacterBanks[2] = mRegister[4] * CHARACTER_BANK_SIZE;
    mCharacterBanks[3] = mRegister[5] * CHARACTER_BANK_SIZE;
    mCharacterBanks[4] = (mRegister[0] & 0xFE) * CHARACTER_BANK_SIZE;
    mCharacterBanks[5] = mRegister[0] * CHARACTER_BANK_SIZE + CHARACTER_BANK_SIZE;
    mCharacterBanks[6] = (mRegister[1] & 0xFE) * CHARACTER_BANK_SIZE;
    mCharacterBanks[7] = mRegister[1] * CHARACTER_BANK_SIZE + CHARACTER_BANK_SIZE;
  } else {
    mCharacterBanks[0] = (mRegister[0] & 0xFE) * CHARACTER_BANK_SIZE;
    mCharacterBanks[1] = mRegister[0] * CHARACTER_BANK_SIZE + CHARACTER_BANK_SIZE;
    mCharacterBanks[2] = (mRegister[1] & 0xFE) * CHARACTER_BANK_SIZE;
    mCharacterBanks[3] = mRegister[1] * CHARACTER_BANK_SIZE + CHARACTER_BANK_SIZE;
    mCharacterBanks[4] = mRegister[2] * CHARACTER_BANK_SIZE;
    mCharacterBanks[5] = mRegister[3] * CHARACTER_BANK_SIZE;
    mCharacterBanks[6] = mRegister[4] * CHARACTER_BANK_SIZE;
    mCharacterBanks[7] = mRegister[5] * CHARACTER_BANK_SIZE;
  }

  const uint32_t registerSix = (mRegister[6] & 0x3F) * PROGRAM_BANK_SIZE;
  const uint32_t registerSeven = (mRegister[7] & 0x3F) * PROGRAM_BANK_SIZE;
  const uint32_t secondLastRegister = (mProgramBankCount * 2 - 2) * PROGRAM_BANK_SIZE;
  const uint32_t lastRegister = (mProgramBankCount * 2 - 1) * PROGRAM_BANK_SIZE;

  if (mProgramBankMode) {
    mProgramBanks[2] = registerSix;
    mProgramBanks[0] = secondLastRegister;
  } else {
    mProgramBanks[0] = registerSix;
    mProgramBanks[2] = secondLastRegister;
  }

  mProgramBanks[1] = registerSeven;
  mProgramBanks[3] = lastRegister;
}

void Mapper004::DetermineInterruptEnable(bool evenAddress) {
  if (evenAddress) {
    mInteruptEnable = false;
    mInteruptActive = false;
  } else {
    mInteruptEnable = true;
  }
}
