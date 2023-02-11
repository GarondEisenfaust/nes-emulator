#pragma once
#include "IOpcode.h"

class TAY : public IOpcode {
 public:
  TAY(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
