#pragma once
#include "IPpuState.h"

class NotVisibleScreenSpaceState : public IPpuState {
 public:
  NotVisibleScreenSpaceState(PixelProcessingUnit& ppu);
  void Execute() override;
};