#pragma once
#include <cstdint>
#include <string>

class Cpu;

class IAddressingMode {
 public:
  IAddressingMode(Cpu* cpu);
  virtual bool operator()() = 0;

 protected:
  Cpu* mCpu;
};