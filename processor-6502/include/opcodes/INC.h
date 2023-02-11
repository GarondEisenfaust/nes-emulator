#pragma once
#include "IOpcode.h"

class INC : public IOpcode {
 public:
  INC(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
