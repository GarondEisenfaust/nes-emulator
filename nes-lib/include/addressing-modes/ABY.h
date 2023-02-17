#pragma once
#include "IAddressingMode.h"

class ABY : public IAddressingMode {
 public:
  ABY(Processor6502* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};