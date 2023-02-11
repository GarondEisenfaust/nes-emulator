#pragma once
#include "IOpcode.h"

class BRK : public IOpcode {
 public:
  BRK(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
