#pragma once
#include "IPpuState.h"

class HorizontalBlankState : public IPpuState {
 public:
  HorizontalBlankState(Ppu& ppu);
  void Execute() override;
};
