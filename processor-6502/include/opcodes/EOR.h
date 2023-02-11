#pragma once
#include "IOpcode.h"

class EOR : public IOpcode {
 public:
  EOR(Processor6502* cpu);
  virtual bool operator()();
};
