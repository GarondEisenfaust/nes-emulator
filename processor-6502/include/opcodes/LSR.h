#pragma once
#include "IOpcode.h"

class LSR : public IOpcode {
 public:
  LSR(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
