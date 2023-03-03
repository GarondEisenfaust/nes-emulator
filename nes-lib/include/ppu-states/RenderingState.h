#pragma once
#include "IPpuState.h"

class RenderingState : public IPpuState {
 public:
  RenderingState(Ppu& ppu);
  void Execute() override;

 private:
  void IncrementScrollX();
  void IncrementScrollY();
  void TransferAddressX();
  void TransferAddressY();
  void LoadBackgroundShifters();
  void UpdateShifters();
};
