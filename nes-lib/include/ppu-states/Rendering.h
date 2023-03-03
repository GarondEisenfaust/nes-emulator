#pragma once
#include "IPpuState.h"

class Rendering : public IPpuState {
 public:
  Rendering(Ppu& ppu);
  void Execute() override;
};
