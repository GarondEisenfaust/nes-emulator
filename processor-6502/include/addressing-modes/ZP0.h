#pragma once
#include "IAddressingMode.h"

class ZP0 : public IAddressingMode {
 public:
  ZP0(Processor6502* cpu);
  virtual bool operator()();
};