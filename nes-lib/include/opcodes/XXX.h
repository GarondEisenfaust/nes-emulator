#pragma once
#include "IOpcode.h"

class XXX : public IOpcode {
 public:
  XXX(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
