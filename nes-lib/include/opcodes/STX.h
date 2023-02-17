#pragma once
#include "IOpcode.h"

class STX : public IOpcode {
 public:
  STX(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
