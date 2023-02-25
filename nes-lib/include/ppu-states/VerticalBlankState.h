#pragma once
#include "IPpuState.h"

class VerticalBlankState : public IPpuState {
 public:
  VerticalBlankState(PixelProcessingUnit& ppu);
  void Execute() override;
};