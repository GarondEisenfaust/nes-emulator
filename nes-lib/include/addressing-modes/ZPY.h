#pragma once
#include "IAddressingMode.h"

class ZPY : public IAddressingMode {
 public:
  ZPY(Cpu* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};