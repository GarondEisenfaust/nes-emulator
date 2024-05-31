#pragma once
class Cpu;

class IOpcode {
 public:
  IOpcode(Cpu* cpu) : mCpu(cpu) {}
  virtual void operator()() = 0;
  virtual const char* Name() = 0;

 protected:
  Cpu* mCpu;
};