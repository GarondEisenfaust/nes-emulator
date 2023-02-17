#pragma once
#include "IOpcode.h"

class PLA : public IOpcode {
 public:
  PLA(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
