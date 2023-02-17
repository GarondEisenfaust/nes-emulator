#pragma once
#include "IOpcode.h"

class PHA : public IOpcode {
 public:
  PHA(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
