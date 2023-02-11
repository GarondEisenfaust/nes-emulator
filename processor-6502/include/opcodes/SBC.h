#pragma once
#include "IOpcode.h"

class SBC : public IOpcode {
 public:
  SBC(Processor6502* cpu);
  virtual bool operator()();
};
