#pragma once
#include "IPpuState.h"

class VisibleScreenSpaceState : public IPpuState {
 public:
  VisibleScreenSpaceState(Ppu& ppu);
  void Execute() override;
};
