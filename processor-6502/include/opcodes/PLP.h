#pragma once
#include "IOpcode.h"

class PLP : public IOpcode {
 public:
  PLP(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
