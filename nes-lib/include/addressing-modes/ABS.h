#pragma once
#include "IAddressingMode.h"

class ABS : public IAddressingMode {
 public:
  ABS(Cpu* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};