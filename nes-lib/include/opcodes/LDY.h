#pragma once
#include "IOpcode.h"

class LDY : public IOpcode {
 public:
  LDY(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
