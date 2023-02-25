#pragma once
#include "IPpuState.h"

class VisibleScreenSpaceState : public IPpuState {
 public:
  VisibleScreenSpaceState(PixelProcessingUnit& ppu);
  void Execute() override;
};
