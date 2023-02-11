#pragma once
#include "IOpcode.h"

class BCS : public IOpcode {
 public:
  BCS(Processor6502* cpu);
  virtual bool operator()();
};
