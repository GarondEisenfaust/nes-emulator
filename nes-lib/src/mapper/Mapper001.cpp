#include "mapper/Mapper001.h"
#include <iostream>
#include <string>

Mapper001::Mapper001(uint8_t programBankCount, uint8_t characterBankCount, MirrorMode mirrorMode)
    : IMapper(programBankCount, characterBankCount, mirrorMode) {}

MappingResult Mapper001::CpuMapRead(uint16_t address) {
  if (address < 0x6000) {
    return {};
  }

  if (0x6000 <= address && address <= 0x7FFF) {
    return {{}, mMapperRam[address & 0x1FFF]};
  }

  if (!(mControlRegister & 0b01000)) {
    return {mProgramBankSelect * 0x8000 + (address & 0x7FFF), {}};
  }

  if (0x8000 <= address && address <= 0xBFFF) {
    return {mProgramBankSelectLower * 0x4000 + (address & 0x3FFF), {}};
  }

  if (0xC000 <= address && address <= 0xFFFF) {
    return {mProgramBankSelectHigher * 0x4000 + (address & 0x3FFF), {}};
  }
  return {};
}

MappingResult Mapper001::CpuMapWrite(uint16_t address, uint8_t data) {
  if (address < 0x6000) {
    return {};
  }

  if (0x6000 <= address && address <= 0x7FFF) {
    mMapperRam[address & 0x1FFF] = data;
    return {};
  }

  if (data & 0x80) {
    mLoadRegister.Reset();
    mControlRegister = mControlRegister | 0x0C;
    return {};
  }

  mLoadRegister.PushIntoLoadRegister(data);

  if (mLoadRegister.Full()) {
    HandleFullLoadRegister(address);
  }
  return {};
}

void Mapper001::HandleFullLoadRegister(uint16_t address) {
  uint8_t targetRegister = (address >> 13) & 0x03;

  if (targetRegister == 0) {
    TargetRegisterZero();
  } else if (targetRegister == 1) {
    TargetRegisterOne();
  } else if (targetRegister == 2) {
    TargetRegisterTwo();
  } else if (targetRegister == 3) {
    TargetRegisterThree();
  }
  mLoadRegister.Reset();
}

MappingResult Mapper001::PpuMapRead(uint16_t address) {
  if (address >= 0x2000) {
    return {};
  }

  if (mCharacterBankCount == 0) {
    return {address, {}};
  }

  if (!(mControlRegister & 0b10000)) {
    return {mCharacterBankSelect * 0x2000 + (address & 0x1FFF), {}};
  }

  if (0x0000 <= address && address <= 0x0FFF) {
    return {mCharacterBankSelectLower * 0x1000 + (address & 0x0FFF), {}};
  }

  if (0x1000 <= address && address <= 0x1FFF) {
    return {mCharacterBankSelectHigher * 0x1000 + (address & 0x0FFF), {}};
  }
  return {};
}

MappingResult Mapper001::PpuMapWrite(uint16_t address) {
  if (address < 0x2000 && mCharacterBankCount == 0) {
    return {address, {}};
  }
  return {};
}

void Mapper001::Reset() {
  mControlRegister = 0x1C;
  mLoadRegister.Reset();

  mCharacterBankSelectLower = 0;
  mCharacterBankSelectHigher = 0;
  mCharacterBankSelect = 0;

  mProgramBankSelect = 0;
  mProgramBankSelectLower = 0;
  mProgramBankSelectHigher = mProgramBankCount - 1;
}

void Mapper001::TargetRegisterZero() {
  mControlRegister = mLoadRegister & 0x1F;
  mMirrorMode = static_cast<MirrorMode>(mControlRegister & 0x03);
}

void Mapper001::TargetRegisterOne() {
  if (mControlRegister & 0b10000) {
    mCharacterBankSelectLower = mLoadRegister & 0x1F;
    return;
  }
  mCharacterBankSelect = mLoadRegister & 0x1E;
}

void Mapper001::TargetRegisterTwo() {
  if (!(mControlRegister & 0b10000)) {
    return;
  }
  mCharacterBankSelectHigher = mLoadRegister & 0x1F;
}

void Mapper001::TargetRegisterThree() {
  uint8_t programMode = (mControlRegister >> 2) & 0x03;

  if (programMode == 0 || programMode == 1) {
    mProgramBankSelect = (mLoadRegister & 0x0E) >> 1;
    return;
  }
  if (programMode == 2) {
    mProgramBankSelectLower = 0;
    mProgramBankSelectHigher = mLoadRegister & 0x0F;
    return;
  }
  if (programMode == 3) {
    mProgramBankSelectLower = mLoadRegister & 0x0F;
    mProgramBankSelectHigher = mProgramBankCount - 1;
    return;
  }
}

void Mapper001::LoadRegister::Reset() {
  mRegister = 0x00;
  mCount = 0;
}

void Mapper001::LoadRegister::PushIntoLoadRegister(uint8_t data) {
  mRegister >>= 1;
  mRegister |= (data & 0x01) << 4;
  mCount++;
}

bool Mapper001::LoadRegister::Full() { return mCount == 5; }

uint8_t Mapper001::LoadRegister::operator&(const uint8_t byte) { return mRegister & byte; }
