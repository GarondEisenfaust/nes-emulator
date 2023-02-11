#pragma once
#include "IOpcode.h"

class ROL : public IOpcode {
 public:
  ROL(Processor6502* cpu);
  virtual bool operator()();
};
