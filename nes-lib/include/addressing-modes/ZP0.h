#pragma once
#include "IAddressingMode.h"

class ZP0 : public IAddressingMode {
 public:
  ZP0(Cpu* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};