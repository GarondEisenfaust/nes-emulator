#pragma once
#include <cstdint>
#include <string>

class Cpu;

class IAddressingMode {
 public:
  IAddressingMode(Cpu* cpu);
  virtual bool operator()() = 0;
  virtual std::string Disassemble(uint32_t& current) = 0;

 protected:
  Cpu* mCpu;
  uint16_t Read16BitAddress(uint32_t& address);
};