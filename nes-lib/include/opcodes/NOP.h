#pragma once
#include "IOpcode.h"

class NOP : public IOpcode {
 public:
  NOP(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
