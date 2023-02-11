#pragma once
class Processor6502;

class IAddressingMode {
 public:
  IAddressingMode(Processor6502* cpu) : mCpu(cpu) {}
  virtual bool operator()() = 0;

 protected:
  Processor6502* mCpu;
};