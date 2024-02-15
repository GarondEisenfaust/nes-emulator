#include "DmcChannel.h"
#include "Apu.h"

DmcChannel::DmcChannel(Apu* apu) : mApu(apu) {
  mRateTable = {214, 190, 170, 160, 143, 127, 113, 107, 95, 80, 71, 64, 53, 42, 36, 27};
}

DmcChannel::~DmcChannel() {}

void DmcChannel::Write(uint16_t addr, uint8_t data) {
  if (addr == 0x4010) {
    mInterruptEnabled = data & (1 << 7);
    if (mInterruptEnabled) {
      mApu->mInterrupt = false;
    }
    mLoop = data & (1 << 6);
    mDivider.SetPeriod(mRateTable[0x0F & data] - 1);
  } else if (addr == 0x4011) {
    output = 0x7F & data;
  } else if (addr == 0x4012) {
    mSampleAddress = 0xC000 + (data * 64);
  } else if (addr == 0x4013) {
    mSampleLength = (data * 16) + 0b0001;
  } else if (addr == 0x4015) {
    bool status = data & 0x10;
    if (status && mBytesRemaining == 0) {
      Restart();
    } else if (!status) {
      mBytesRemaining = 0;
    }
    mApu->mInterrupt = false;
  }
}

void DmcChannel::Clock() {
  if (mShiftRegisterRemaining == 0 && mBytesRemaining > 0) {
    mSampleBuffer = ReadSample(mCurrentSampleAddress);
    if (mBytesRemaining == 0) {
      if (mLoop) {
        Restart();
      } else if (mInterruptEnabled) {
        mApu->mInterrupt = true;
      }
    }
  }

  if (mShiftRegisterRemaining == 0) {
    mShiftRegisterRemaining = 8;
    if (mSampleBuffer == 0) {
      mSilence = true;
    } else {
      mSilence = false;
      mShiftRegister = mSampleBuffer;
      mSampleBuffer = 0;
    }
  }

  mDivider.Clock();
  if (mDivider.Notify()) {
    mDivider.Reset();

    if (!mSilence) {
      bool mShiftRegisterOutput = mShiftRegister & 0x01;
      if (mShiftRegisterOutput && (output <= 125)) {
        output += 2;
      } else if (!mShiftRegisterOutput && (output >= 2)) {
        output -= 2;
      }
    }
    mShiftRegister >>= 1;
    mShiftRegisterRemaining--;
  }
}

void DmcChannel::Restart() {
  mCurrentSampleAddress = mSampleAddress;
  mBytesRemaining = mSampleLength;
}

void DmcChannel::IncrementCurrentSampleAddress() {
  if (mCurrentSampleAddress == 0xFFFF) {
    mCurrentSampleAddress = 0x8000;
  }
  mCurrentSampleAddress++;
}

uint8_t DmcChannel::ReadSample(uint16_t addr) {
  const uint8_t sample = mApu->ApuRead(addr);
  IncrementCurrentSampleAddress();
  mBytesRemaining--;
  return sample;
}
