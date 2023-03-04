#pragma once
#include "IPpuState.h"

class RenderingState : public IPpuState {
 public:
  RenderingState(Ppu& ppu);
  void Execute() override;
};
