#pragma once
#include "IOpcode.h"

class CLC : public IOpcode {
 public:
  CLC(Processor6502* cpu);
  virtual bool operator()();
};
