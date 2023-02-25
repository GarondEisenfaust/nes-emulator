#pragma once
#include "IPpuState.h"

class VerticalBlankState : public IPpuState {
 public:
  VerticalBlankState(Ppu& ppu);
  void Execute() override;
};