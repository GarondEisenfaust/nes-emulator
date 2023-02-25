#pragma once

class Ppu;
class IPpuState {
 public:
  IPpuState(Ppu& ppu);
  virtual void Execute() = 0;

 protected:
  Ppu& mPpu;
};