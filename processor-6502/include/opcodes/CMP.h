#pragma once
#include "IOpcode.h"

class CMP : public IOpcode {
 public:
  CMP(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
