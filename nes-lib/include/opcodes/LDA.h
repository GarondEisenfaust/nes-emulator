#pragma once
#include "IOpcode.h"

class LDA : public IOpcode {
 public:
  LDA(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
