#pragma once
#include "IOpcode.h"

class LDX : public IOpcode {
 public:
  LDX(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
