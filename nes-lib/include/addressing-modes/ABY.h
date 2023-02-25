#pragma once
#include "IAddressingMode.h"

class ABY : public IAddressingMode {
 public:
  ABY(Cpu* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};