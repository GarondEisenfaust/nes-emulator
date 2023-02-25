#pragma once
class Cpu;

class IOpcode {
 public:
  IOpcode(Cpu* cpu) : mCpu(cpu) {}
  virtual bool operator()() = 0;
  virtual const char* Name() = 0;

 protected:
  Cpu* mCpu;
};