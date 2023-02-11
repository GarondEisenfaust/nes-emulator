#pragma once
#include "IOpcode.h"

class RTI : public IOpcode {
 public:
  RTI(Processor6502* cpu);
  virtual bool operator()();
};
