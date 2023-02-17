#pragma once
#include "IAddressingMode.h"

class IMM : public IAddressingMode {
 public:
  IMM(Processor6502* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};