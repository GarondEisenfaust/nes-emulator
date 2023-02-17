#pragma once
#include "IOpcode.h"

class SEI : public IOpcode {
 public:
  SEI(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
