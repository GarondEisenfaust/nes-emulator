#pragma once
#include "IOpcode.h"

class CLI : public IOpcode {
 public:
  CLI(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
