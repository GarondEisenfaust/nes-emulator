#pragma once
#include "IAddressingMode.h"

class IZY : public IAddressingMode {
 public:
  IZY(Cpu* cpu);
  virtual bool operator()();
  virtual std::string Disassemble(uint32_t& current);
};