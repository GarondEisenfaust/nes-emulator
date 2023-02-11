#pragma once
#include <cstdint>
#include <string>

class Processor6502;

class IAddressingMode {
 public:
  IAddressingMode(Processor6502* cpu) : mCpu(cpu) {}
  virtual bool operator()() = 0;
  virtual std::string Disassemble(uint32_t& current) = 0;

 protected:
  Processor6502* mCpu;
};