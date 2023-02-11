#pragma once
#include "IOpcode.h"

class SED : public IOpcode {
 public:
  SED(Processor6502* cpu);
  virtual bool operator()();
};
