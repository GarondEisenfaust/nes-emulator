#pragma once
#include "IOpcode.h"

class ASL : public IOpcode {
 public:
  ASL(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
