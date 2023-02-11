#pragma once
#include "IOpcode.h"

class DEX : public IOpcode {
 public:
  DEX(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
