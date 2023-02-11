#pragma once
class Processor6502;

class IOpcode {
 public:
  IOpcode(Processor6502* cpu) : mCpu(cpu) {}
  virtual bool operator()() = 0;

 protected:
  Processor6502* mCpu;
};