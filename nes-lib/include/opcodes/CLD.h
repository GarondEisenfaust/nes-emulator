#pragma once
#include "IOpcode.h"

class CLD : public IOpcode {
 public:
  CLD(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
