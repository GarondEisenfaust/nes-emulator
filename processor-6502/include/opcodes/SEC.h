#pragma once
#include "IOpcode.h"

class SEC : public IOpcode {
 public:
  SEC(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
