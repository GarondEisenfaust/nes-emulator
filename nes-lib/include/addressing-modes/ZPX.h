#pragma once
#include "IAddressingMode.h"

class ZPX : public IAddressingMode {
 public:
  ZPX(Processor6502* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};