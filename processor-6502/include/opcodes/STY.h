#pragma once
#include "IOpcode.h"

class STY : public IOpcode {
 public:
  STY(Processor6502* cpu);
  virtual bool operator()();
};
