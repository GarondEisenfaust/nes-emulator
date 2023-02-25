#pragma once

class PixelProcessingUnit;
class IPpuState {
 public:
  IPpuState(PixelProcessingUnit& ppu);
  virtual void Execute() = 0;

 protected:
  PixelProcessingUnit& mPpu;
};