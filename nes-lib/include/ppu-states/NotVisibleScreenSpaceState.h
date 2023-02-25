#pragma once
#include "IPpuState.h"

class NotVisibleScreenSpaceState : public IPpuState {
 public:
  NotVisibleScreenSpaceState(Ppu& ppu);
  void Execute() override;
};