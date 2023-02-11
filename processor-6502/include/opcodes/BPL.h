#pragma once
#include "IOpcode.h"

class BPL : public IOpcode {
 public:
  BPL(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
