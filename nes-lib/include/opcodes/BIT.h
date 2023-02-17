#pragma once
#include "IOpcode.h"

class BIT : public IOpcode {
 public:
  BIT(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
