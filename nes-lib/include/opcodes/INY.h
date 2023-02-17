#pragma once
#include "IOpcode.h"

class INY : public IOpcode {
 public:
  INY(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
