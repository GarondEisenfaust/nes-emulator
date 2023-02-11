#pragma once
#include "IOpcode.h"

class CPX : public IOpcode {
 public:
  CPX(Processor6502* cpu);
  virtual bool operator()();
};
