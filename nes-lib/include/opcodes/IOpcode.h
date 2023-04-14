#pragma once
class Cpu;

class IOpcode {
 public:
  IOpcode(Cpu* cpu) : mCpu(cpu) {}
  virtual bool operator()() = 0;

 protected:
  Cpu* mCpu;
};