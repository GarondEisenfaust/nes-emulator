#pragma once
#include "IPpuState.h"

class VerticalBlankStateBegin : public IPpuState {
 public:
  VerticalBlankStateBegin(Ppu& ppu);
  void Execute() override;
};