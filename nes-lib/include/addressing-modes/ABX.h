#pragma once
#include "IAddressingMode.h"

class ABX : public IAddressingMode {
 public:
  ABX(Processor6502* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};