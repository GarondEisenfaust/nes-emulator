#pragma once
#include "IOpcode.h"

class BEQ : public IOpcode {
 public:
  BEQ(Processor6502* cpu);
  virtual bool operator()();
};
