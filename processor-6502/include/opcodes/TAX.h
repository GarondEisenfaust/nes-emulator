#pragma once
#include "IOpcode.h"

class TAX : public IOpcode {
 public:
  TAX(Processor6502* cpu);
  virtual bool operator()();
};
