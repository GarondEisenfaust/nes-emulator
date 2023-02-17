#pragma once
#include "IOpcode.h"

class BNE : public IOpcode {
 public:
  BNE(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
