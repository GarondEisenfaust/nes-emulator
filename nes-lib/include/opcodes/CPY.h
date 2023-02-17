#pragma once
#include "IOpcode.h"

class CPY : public IOpcode {
 public:
  CPY(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
