#pragma once
#include "IOpcode.h"

class ROR : public IOpcode {
 public:
  ROR(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
