#pragma once
#include "IOpcode.h"

class BVS : public IOpcode {
 public:
  BVS(Processor6502* cpu);
  virtual bool operator()();
};
